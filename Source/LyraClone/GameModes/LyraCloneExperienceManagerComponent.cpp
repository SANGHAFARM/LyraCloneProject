#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneExperienceDefinition.h"
#include "LyraClone/System/LyraCloneAssetManager.h"
#include "LyraCloneExperienceActionSet.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameFeaturesSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneExperienceManagerComponent)

ULyraCloneExperienceManagerComponent::ULyraCloneExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraCloneExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void ULyraCloneExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	ULyraCloneAssetManager& AssetManager = ULyraCloneAssetManager::Get();

	TSubclassOf<ULyraCloneExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const ULyraCloneExperienceDefinition* Experience = GetDefault<ULyraCloneExperienceDefinition>(AssetClass);

	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void ULyraCloneExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience != nullptr);
	check(LoadState == ELyraCloneExperienceLoadState::Unloaded);

	LoadState = ELyraCloneExperienceLoadState::Loading;

	ULyraCloneAssetManager& AssetManager = ULyraCloneAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
	// Load assets associated with the experience

	TArray<FName> BundlesToLoad;
	{
		//@TODO: Centralize this client/server stuff into the LyraAssetManager
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

		// If both async loads are running, combine them
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// Assets were already loaded, call the delegate now
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;

	// This set of assets gets preloaded, but we don't block the start of the experience based on it
	TSet<FPrimaryAssetId> PreloadAssetList;
	//@TODO: Determine assets to preload (but not blocking-ly)
	if (PreloadAssetList.Num() > 0)
	{
		AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
	}
}

void ULyraCloneExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check(LoadState == ELyraCloneExperienceLoadState::Loading);
	check(CurrentExperience);

	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
			}
		};

	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = ELyraCloneExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();
	}
}

void ULyraCloneExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELyraCloneExperienceLoadState::Loaded);
	{
		LoadState = ELyraCloneExperienceLoadState::ExecutingActions;

		FGameFeatureActivatingContext Context;

		{
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
			{
				for (UGameFeatureAction* Action : ActionList)
				{
					if (Action)
					{
						Action->OnGameFeatureRegistering();
						Action->OnGameFeatureLoading();
						Action->OnGameFeatureActivating(Context);
					}
				}
			};

		ActivateListOfActions(CurrentExperience->Actions);

		for (const TObjectPtr<ULyraCloneExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}
	LoadState = ELyraCloneExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();

}

const ULyraCloneExperienceDefinition* ULyraCloneExperienceManagerComponent::GetCurrentExperienceChecked() const
{	
	check(LoadState == ELyraCloneExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);

	return CurrentExperience;
}

void ULyraCloneExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginsLoading--;

	if (NumGameFeaturePluginsLoading == 0)
	{
		OnExperienceFullLoadCompleted();
	}
}

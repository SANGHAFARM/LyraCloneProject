#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"

#include "LyraCloneExperienceManagerComponent.generated.h"

class ULyraCloneExperienceDefinition;

enum class ELyraCloneExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraCloneExperienceLoaded, const ULyraCloneExperienceDefinition*);

UCLASS()
class ULyraCloneExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	ULyraCloneExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool IsExperienceLoaded() const { return (LoadState == ELyraCloneExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	void CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();

	const ULyraCloneExperienceDefinition* GetCurrentExperienceChecked() const;

	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);

	UPROPERTY()
	TObjectPtr<const ULyraCloneExperienceDefinition> CurrentExperience;

	ELyraCloneExperienceLoadState LoadState = ELyraCloneExperienceLoadState::Unloaded;

	FOnLyraCloneExperienceLoaded OnExperienceLoaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};

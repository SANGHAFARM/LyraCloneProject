#include "LyraCloneAssetManager.h"
#include "LyraClone/LyraCloneLogChannels.h"
#include "LyraClone/LyraCloneGameplayTags.h"
// #4
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAssetManager)

ULyraCloneAssetManager::ULyraCloneAssetManager()
// #5
	: Super()
{


}

ULyraCloneAssetManager& ULyraCloneAssetManager::Get()
{
	check(GEngine);

	if (ULyraCloneAssetManager* Singleton = Cast<ULyraCloneAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	//UE_LOG(LogLyraClone, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to LyraAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<ULyraCloneAssetManager>();
}

void ULyraCloneAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
bool ULyraCloneAssetManager::TestClone()
{
	static bool bTest = ShouldLogAssetLoads();

	return true;
}

bool ULyraCloneAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}
PRAGMA_ENABLE_OPTIMIZATION

UObject* ULyraCloneAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void ULyraCloneAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(LogLyraClone, Warning, TEXT("ULyraCloneAssetManager::StartInitialLoading"));

	FLyraCloneGameplayTags::InitializeNativeTags();
}

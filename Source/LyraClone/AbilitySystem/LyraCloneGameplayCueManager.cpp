#include "LyraCloneGameplayCueManager.h"

#include "AbilitySystemGlobals.h"
#include "Engine/AssetManager.h"
#include "GameplayCueSet.h"

ULyraCloneGameplayCueManager* ULyraCloneGameplayCueManager::Get()
{
	return Cast<ULyraCloneGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

ULyraCloneGameplayCueManager::ULyraCloneGameplayCueManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));

void ULyraCloneGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}

	FAssetBundleData BundleData;

	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);

}
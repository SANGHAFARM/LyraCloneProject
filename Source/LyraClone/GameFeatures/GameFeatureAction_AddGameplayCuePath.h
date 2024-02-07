#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureStateChangeObserver.h"

#include "GameFeatureAction_AddGameplayCuePath.generated.h"

UCLASS()
class UGameFeatureAction_AddGameplayCuePath : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UGameFeatureAction_AddGameplayCuePath();

	UPROPERTY(EditAnywhere, Category = "GameFeature|GameplayCues")
	TArray<FDirectoryPath> DirectoryPathsToAdd;
};UCLASS()
class ULyraCloneGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString&	PluginName, const FString& PluginURL) override;
};
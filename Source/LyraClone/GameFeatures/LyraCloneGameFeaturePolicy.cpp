#include "LyraCloneGameFeaturePolicy.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"
#include "AbilitySystemGlobals.h"
#include "GameFeatureAction_AddGameplayCuePath.h"
#include "LyraClone/AbilitySystem/LyraCloneGameplayCueManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameFeaturePolicy)

ULyraCloneGameplayFeaturePolicy::ULyraCloneGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ULyraCloneGameplayFeaturePolicy::InitGameFeatureManager()
{
	Observers.Add(NewObject<ULyraCloneGameFeature_AddGameplayCuePaths>());
	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.AddObserver(Observer);
	}
	Super::InitGameFeatureManager();
}

void ULyraCloneGameplayFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();
	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

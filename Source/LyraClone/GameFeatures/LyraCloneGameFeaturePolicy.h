#pragma once

#include "Containers/Array.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "LyraCloneGameFeaturePolicy.generated.h"

UCLASS()
class ULyraCloneGameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
public:
	ULyraCloneGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGameFeatureManager() override;

	virtual void ShutdownGameFeatureManager() override;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};
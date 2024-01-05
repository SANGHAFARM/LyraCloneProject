#pragma once

#include "Abilities/GameplayAbility.h"

#include "LyraCloneGameplayAbility.generated.h"

UCLASS(Abstract)
class ULyraCloneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULyraCloneGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
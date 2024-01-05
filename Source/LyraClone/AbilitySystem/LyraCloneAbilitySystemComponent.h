#pragma once

#include "AbilitySystemComponent.h"

#include "LyraCloneAbilitySystemComponent.generated.h"

UCLASS()
class ULyraCloneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	ULyraCloneAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
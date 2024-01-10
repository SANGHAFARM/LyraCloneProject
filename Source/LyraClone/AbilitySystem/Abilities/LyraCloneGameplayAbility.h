#pragma once

#include "Abilities/GameplayAbility.h"

#include "LyraCloneGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ELyraCloneAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn,
};

UCLASS(Abstract)
class ULyraCloneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULyraCloneGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|AbilityActivation")
	ELyraCloneAbilityActivationPolicy ActivationPolicy;
};
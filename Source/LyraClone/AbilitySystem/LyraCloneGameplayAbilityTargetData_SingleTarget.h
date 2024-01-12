#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"

#include "LyraCloneGameplayAbilityTargetData_SingleTarget.generated.h"

USTRUCT()
struct FLyraCloneGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FLyraCloneGameplayAbilityTargetData_SingleTargetHit() : CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FLyraCloneGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	UPROPERTY()
	int32 CartridgeID;
};
#pragma once

#include "CoreMinimal.h"
#include "LyraClone/Equipment/LyraCloneGameplayAbility_FromEquipment.h"

#include "LyraCloneGameplayAbility_RangedWeapon.generated.h"

UCLASS()
class ULyraCloneGameplayAbility_RangedWeapon : public ULyraCloneGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:

	ULyraCloneGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
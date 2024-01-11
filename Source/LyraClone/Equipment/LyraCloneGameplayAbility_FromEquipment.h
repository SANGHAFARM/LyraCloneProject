#pragma once

#include "CoreMinimal.h"
#include "LyraClone/AbilitySystem/Abilities/LyraCloneGameplayAbility.h"

#include "LyraCloneGameplayAbility_FromEquipment.generated.h"

class ULyraCloneEquipmentInstance;

UCLASS()
class ULyraCloneGameplayAbility_FromEquipment : public ULyraCloneGameplayAbility
{

	GENERATED_BODY()

public:
	ULyraCloneGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraCloneEquipmentInstance* GetAssociatedEquipment() const;

};
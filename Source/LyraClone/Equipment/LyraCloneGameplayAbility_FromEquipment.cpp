#include "LyraCloneGameplayAbility_FromEquipment.h"

#include "LyraCloneEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameplayAbility_FromEquipment)

ULyraCloneGameplayAbility_FromEquipment::ULyraCloneGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULyraCloneEquipmentInstance* ULyraCloneGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<ULyraCloneEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

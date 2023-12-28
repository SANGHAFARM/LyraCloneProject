#include "LyraCloneEquipmentManagerComponent.h"
#include "LyraCloneEquipmentDefinition.h"
#include "LyraCloneEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneEquipmentManagerComponent)

ULyraCloneEquipmentManagerComponent::ULyraCloneEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EquipmentList(this)
{
}

#pragma once

#include "Components/ControllerComponent.h"

#include "LyraCloneQuickBarComponent.generated.h"

class ULyraCloneEquipmentInstance;
class ULyraCloneEquipmentManagerComponent;
class ULyraCloneInventoryItemInstance;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ULyraCloneQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	ULyraCloneQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneInventoryItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<ULyraCloneEquipmentInstance> EquippedItem;
};
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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, ULyraCloneInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "LyraClone")
	void SetActiveSlotIndex(int32 NewIndex);

	ULyraCloneEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();

	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneInventoryItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<ULyraCloneEquipmentInstance> EquippedItem;
};
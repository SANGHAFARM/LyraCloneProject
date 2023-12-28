#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Array.h"

#include "LyraCloneEquipmentInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ULyraCloneEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 어떤 InventoryItemInstance에 의해 활성화되었는지 (추후 QuickBarComponent에서 보게될 것) */
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	/** LyraCloneEquipmentDefinition에 맞게 Spawn된 Actor Instance들 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

#pragma once

#include "Components/PawnComponent.h"

#include "LyraCloneEquipmentManagerComponent.generated.h"

class ULyraCloneEquipmentDefinition;
class ULyraCloneEquipmentInstance;

USTRUCT(BlueprintType)
struct FLyraCloneAppliedEquipmentEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<ULyraCloneEquipmentInstance> Instance = nullptr;

};

USTRUCT(BlueprintType)
struct FLyraCloneEquipmentList
{
	GENERATED_BODY()

	FLyraCloneEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{
	}

	UPROPERTY()
	TArray<FLyraCloneAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS(BlueprintType, Const)
class ULyraCloneEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULyraCloneEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FLyraCloneEquipmentList EquipmentList;

};
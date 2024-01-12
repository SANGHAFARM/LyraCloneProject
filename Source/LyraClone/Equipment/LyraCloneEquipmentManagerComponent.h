#pragma once

#include "Components/PawnComponent.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySet.h"

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

	UPROPERTY()
	FLyraCloneAbilitySet_GrantedHandles GrantedHandles;
};

USTRUCT(BlueprintType)
struct FLyraCloneEquipmentList
{
	GENERATED_BODY()

	FLyraCloneEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{
	}

	ULyraCloneEquipmentInstance* AddEntry(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULyraCloneEquipmentInstance* Instance);

	ULyraCloneAbilitySystemComponent* GetAbilitySystemComponent() const;

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

	ULyraCloneEquipmentInstance* EquipItem(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULyraCloneEquipmentInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	TArray<ULyraCloneEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<ULyraCloneEquipmentInstance> InstanceType) const;

	UPROPERTY()
	FLyraCloneEquipmentList EquipmentList;

};
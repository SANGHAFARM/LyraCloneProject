#include "LyraCloneEquipmentManagerComponent.h"

#include "LyraCloneEquipmentDefinition.h"
#include "LyraCloneEquipmentInstance.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneEquipmentManagerComponent)

ULyraCloneEquipmentManagerComponent::ULyraCloneEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EquipmentList(this)
{
}

ULyraCloneEquipmentInstance* ULyraCloneEquipmentManagerComponent::EquipItem(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition)
{
	ULyraCloneEquipmentInstance* Result = nullptr;

	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void ULyraCloneEquipmentManagerComponent::UnequipItem(ULyraCloneEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

ULyraCloneEquipmentInstance* FLyraCloneEquipmentList::AddEntry(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition)
{
	ULyraCloneEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const ULyraCloneEquipmentDefinition* EquipmentCDO =	GetDefault<ULyraCloneEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<ULyraCloneEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = ULyraCloneEquipmentInstance::StaticClass();
	}

	FLyraCloneAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULyraCloneEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	ULyraCloneAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	{
		for (TObjectPtr<ULyraCloneAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	return Result;
}

void FLyraCloneEquipmentList::RemoveEntry(ULyraCloneEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraCloneAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			ULyraCloneAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

ULyraCloneAbilitySystemComponent* FLyraCloneEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	return Cast<ULyraCloneAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

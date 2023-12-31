#include "LyraCloneInventoryManagerComponent.h"
#include "LyraCloneInventoryItemDefinition.h"
#include "LyraCloneInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneInventoryManagerComponent)

ULyraCloneInventoryManagerComponent::ULyraCloneInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), InventoryList(this)
{
}

ULyraCloneInventoryItemInstance* ULyraCloneInventoryManagerComponent::AddItemDefinition(TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef)
{
	ULyraCloneInventoryItemInstance* Result = nullptr;

	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}

	return Result;
}

ULyraCloneInventoryItemInstance* FLyraCloneInventoryList::AddEntry(TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef)
{
	ULyraCloneInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FLyraCloneInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<ULyraCloneInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	Result = NewEntry.Instance;

	return Result;
}

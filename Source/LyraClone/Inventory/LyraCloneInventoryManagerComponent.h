#pragma once

#include "Components/ActorComponent.h"

#include "LyraCloneInventoryManagerComponent.generated.h"

class ULyraCloneInventoryItemInstance;
class ULyraCloneInventoryItemDefinition;

/** Inventory Item 단위 객체 */
USTRUCT(BlueprintType)
struct FLyraCloneInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ULyraCloneInventoryItemInstance> Instance = nullptr;
};

/** Inventory Item 관리 객체 */
USTRUCT(BlueprintType)
struct FLyraCloneInventoryList
{
	GENERATED_BODY()

	FLyraCloneInventoryList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{
	}

	ULyraCloneInventoryItemInstance* AddEntry(TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef);

	UPROPERTY()
	TArray<FLyraCloneInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

};

/** PlayerController의 Component로서 Inventory를 관리한다 */
UCLASS(BlueprintType)
class ULyraCloneInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULyraCloneInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ULyraCloneInventoryItemInstance* AddItemDefinition(TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef);

	UPROPERTY()
	FLyraCloneInventoryList InventoryList;

};
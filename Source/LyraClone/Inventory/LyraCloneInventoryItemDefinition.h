#pragma once

#include "LyraCloneInventoryItemDefinition.generated.h"


UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class ULyraCloneInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:

};

UCLASS(Blueprintable)
class ULyraCloneInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, Instanced)
	TArray<TObjectPtr<ULyraCloneInventoryItemFragment>> Fragments;

};
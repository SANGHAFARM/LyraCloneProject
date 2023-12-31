#pragma once

#include "Templates/SubclassOf.h"
#include "UObject/Object.h"

#include "LyraCloneInventoryItemInstance.generated.h"

class ULyraCloneInventoryItemFragment;
class ULyraCloneInventoryItemDefinition;

UCLASS(BlueprintType)
class ULyraCloneInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const ULyraCloneInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraCloneInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>	
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	UPROPERTY()
	TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef;
};
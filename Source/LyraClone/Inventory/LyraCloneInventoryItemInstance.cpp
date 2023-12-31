#include "LyraCloneInventoryItemInstance.h"
#include "LyraCloneInventoryItemDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneInventoryItemInstance)

ULyraCloneInventoryItemInstance::ULyraCloneInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const ULyraCloneInventoryItemFragment* ULyraCloneInventoryItemInstance::FindFragmentByClass(TSubclassOf<ULyraCloneInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<ULyraCloneInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

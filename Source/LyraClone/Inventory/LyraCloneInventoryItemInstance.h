#pragma once

#include "Templates/SubclassOf.h"
#include "UObject/Object.h"

#include "LyraCloneInventoryItemInstance.generated.h"

class ULyraCloneInventoryItemInstance;

UCLASS(BlueprintType)
class ULyraCloneInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TSubclassOf<ULyraCloneInventoryItemInstance> ItemDef;
};
#pragma once

#include "Components/PawnComponent.h"

#include "LyraClonePawnComponent_CharacterParts.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class ULyraClonePawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULyraClonePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


};
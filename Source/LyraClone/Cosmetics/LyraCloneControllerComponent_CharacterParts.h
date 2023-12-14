#pragma once

#include "LyraCloneCharacterPartTypes.h"
#include "Components/ControllerComponent.h"

#include "LyraCloneControllerComponent_CharacterParts.generated.h"

USTRUCT()
struct FLyraCloneControllerCharacterPartEntry
{
	GENERATED_BODY()

	// The character part being represented
	UPROPERTY(EditAnywhere)
	FLyraCloneCharacterPart Part;

	// The handle if already applied to a pawn
	FLyraCloneCharacterPartHandle Handle;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class ULyraCloneControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()

public:
	ULyraCloneControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FLyraCloneControllerCharacterPartEntry> CharacterParts;



};
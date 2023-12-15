#pragma once

#include "Components/PawnComponent.h"
#include "LyraCloneCharacterPartTypes.h"
#include "LyraCloneCosmeticAnimationTypes.h"

#include "LyraClonePawnComponent_CharacterParts.generated.h"

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FLyraCloneAppliedCharacterPartEntry
{
	GENERATED_BODY()

	FLyraCloneAppliedCharacterPartEntry()
	{}
		
	// The character part being represented
	UPROPERTY()
	FLyraCloneCharacterPart Part;

	// Handle index we returned to the user (server only)
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// The spawned actor instance (client only)
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FLyraCloneCharacterPartList
{
	GENERATED_BODY()

	FLyraCloneCharacterPartList()
		: OwnerComponent(nullptr)
	{
	}

	FLyraCloneCharacterPartList(ULyraClonePawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	FLyraCloneCharacterPartHandle AddEntry(FLyraCloneCharacterPart NewPart);

	bool SpawnActorForEntry(FLyraCloneAppliedCharacterPartEntry& Entry);

	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FLyraCloneAppliedCharacterPartEntry> Entries;

	// The component that contains this list
	UPROPERTY()
	TObjectPtr<ULyraClonePawnComponent_CharacterParts> OwnerComponent;

	// Upcounter for handles
	int32 PartHandleCounter = 0;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class ULyraClonePawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULyraClonePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
		
	FLyraCloneCharacterPartHandle AddCharacterPart(const FLyraCloneCharacterPart& NewPart);

	// List of character parts
	UPROPERTY()
	FLyraCloneCharacterPartList CharacterPartList;

	// Rules for how to pick a body style mesh for animation to play on, based on character part cosmetics tags
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FLyraCloneAnimBodyStyleSelectionSet BodyMeshes;

};
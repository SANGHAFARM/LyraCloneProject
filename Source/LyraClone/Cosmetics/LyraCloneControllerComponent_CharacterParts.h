#pragma once

#include "LyraCloneCharacterPartTypes.h"
#include "Components/ControllerComponent.h"

#include "LyraCloneControllerComponent_CharacterParts.generated.h"

class ULyraClonePawnComponent_CharacterParts;

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

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;	

	ULyraClonePawnComponent_CharacterParts* GetPawnCustomizer() const;
	void AddCharacterPartInternal(const FLyraCloneCharacterPart& NewPart);

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FLyraCloneCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FLyraCloneControllerCharacterPartEntry> CharacterParts;



};
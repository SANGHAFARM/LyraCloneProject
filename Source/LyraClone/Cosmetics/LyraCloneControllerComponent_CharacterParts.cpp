#include "LyraCloneControllerComponent_CharacterParts.h"
#include "LyraClonePawnComponent_CharacterParts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneControllerComponent_CharacterParts)

ULyraCloneControllerComponent_CharacterParts::ULyraCloneControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

ULyraClonePawnComponent_CharacterParts* ULyraCloneControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<ULyraClonePawnComponent_CharacterParts>();
	}
	return nullptr;
}

void ULyraCloneControllerComponent_CharacterParts::AddCharacterPartInternal(const FLyraCloneCharacterPart& NewPart)
{
	FLyraCloneControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (ULyraClonePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void ULyraCloneControllerComponent_CharacterParts::AddCharacterPart(const FLyraCloneCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

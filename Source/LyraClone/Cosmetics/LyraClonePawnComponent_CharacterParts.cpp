
#include "LyraClonePawnComponent_CharacterParts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePawnComponent_CharacterParts)


bool FLyraCloneCharacterPartList::SpawnActorForEntry(FLyraCloneAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActors = false;

	if (Entry.Part.PartClass != nullptr)
	{
		UWorld* World = OwnerComponent->GetWorld();

		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());

			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);
			PartComponent->RegisterComponent();

			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				// Set up a direct tick dependency to work around the child actor component not providing one
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}

			Entry.SpawnedComponent = PartComponent;
			bCreatedAnyActors = true;
		}
	}

	return bCreatedAnyActors;
}

FLyraCloneCharacterPartHandle FLyraCloneCharacterPartList::AddEntry(FLyraCloneCharacterPart NewPart)
{
	FLyraCloneCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FLyraCloneAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;

		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

ULyraClonePawnComponent_CharacterParts::ULyraClonePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FLyraCloneCharacterPartHandle ULyraClonePawnComponent_CharacterParts::AddCharacterPart(const FLyraCloneCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

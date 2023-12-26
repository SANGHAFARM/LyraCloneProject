
#include "LyraClonePawnComponent_CharacterParts.h"

#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"

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

void FLyraCloneCharacterPartList::DestroyActorForEntry(FLyraCloneAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent != nullptr)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FGameplayTagContainer FLyraCloneCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	for (const FLyraCloneAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent != nullptr)
		{
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(/*inout*/ Result);
			}
		}
	}

	return Result;
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

void FLyraCloneCharacterPartList::RemoveEntry(FLyraCloneCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraCloneAppliedCharacterPartEntry& Entry = *EntryIt;

		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
		}
	}
}

ULyraClonePawnComponent_CharacterParts::ULyraClonePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), CharacterPartList(this)
{

}

USkeletalMeshComponent* ULyraClonePawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}

	return nullptr;
}

USceneComponent* ULyraClonePawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}

	return nullptr;
}

FGameplayTagContainer ULyraClonePawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		return Result;
	}
}

void ULyraClonePawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// Check to see if the body type has changed
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// Determine the mesh to use based on cosmetic part tags
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		// Apply the desired mesh (this call is a no-op if the mesh hasn't changed)
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		// Apply the desired physics asset if there's a forced override independent of the one from the mesh
		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}

FLyraCloneCharacterPartHandle ULyraClonePawnComponent_CharacterParts::AddCharacterPart(const FLyraCloneCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void ULyraClonePawnComponent_CharacterParts::RemoveCharacterPart(FLyraCloneCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}
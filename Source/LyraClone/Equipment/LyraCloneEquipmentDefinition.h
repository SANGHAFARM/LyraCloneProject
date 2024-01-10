// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "LyraCloneEquipmentDefinition.generated.h"

class AActor;
class ULyraCloneAbilitySet;
class ULyraCloneEquipmentInstance;

USTRUCT()
struct FLyraCloneEquipmentActorToSpawn
{
	GENERATED_BODY()

	FLyraCloneEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};


/**
 * ULyraEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, BlueprintType)
class ULyraCloneEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULyraCloneEquipmentInstance> InstanceType;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FLyraCloneEquipmentActorToSpawn> ActorsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<ULyraCloneAbilitySet>> AbilitySetsToGrant;
};

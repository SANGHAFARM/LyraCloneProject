// Copyright Epic Games, Inc. All Rights Reserved.


#include "LyraCloneGameMode.h"
#include "LyraCloneGameState.h"
#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneExperienceDefinition.h"
#include "LyraClone/Character/LyraCloneCharacter.h"
#include "LyraClone/Character/LyraClonePawnData.h"
#include "LyraClone/Character/LyraClonePawnExtensionComponent.h"
#include "LyraClone/Player/LyraClonePlayerController.h"
#include "LyraClone/Player/LyraClonePlayerState.h"
#include "LyraClone/LyraCloneLogChannels.h"

#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameMode)

ALyraCloneGameMode::ALyraCloneGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ALyraCloneGameState::StaticClass();
	PlayerControllerClass = ALyraClonePlayerController::StaticClass();
	PlayerStateClass = ALyraClonePlayerState::StaticClass();
	DefaultPawnClass = ALyraCloneCharacter::StaticClass();
}

void ALyraCloneGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALyraCloneGameMode::InitGameState()
{
	Super::InitGameState();

	ULyraCloneExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceComponent);

	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

APawn* ALyraCloneGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent 구현
			if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const ULyraClonePawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

void ALyraCloneGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

UClass* ALyraCloneGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const ULyraClonePawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ALyraCloneGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;

	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(ULyraCloneExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	}

	if (!ExperienceId.IsValid())
	{
		//@TODO: Pull this from a config setting or something
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LyraCloneExperienceDefinition"), FName("B_LyraCloneDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void ALyraCloneGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());
		
	ULyraCloneExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->ServerSetCurrentExperience(ExperienceId);
}

bool ALyraCloneGameMode::IsExperienceLoaded() const
{
	check(GameState);
	ULyraCloneExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}

void ALyraCloneGameMode::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{
	// Spawn any players that are already attached
	//@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
	// GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const ULyraClonePawnData* ALyraCloneGameMode::GetPawnDataForController(const AController* InController) const
{
	if (InController != nullptr)
	{
		if (const ALyraClonePlayerState* LyraPS = InController->GetPlayerState<ALyraClonePlayerState>())
		{
			if (const ULyraClonePawnData* PawnData = LyraPS->GetPawnData<ULyraClonePawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	ULyraCloneExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const ULyraCloneExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}
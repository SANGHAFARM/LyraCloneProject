#include "LyraClonePlayerState.h"

#include "LyraClone/GameModes/LyraCloneExperienceDefinition.h"
#include "LyraClone/GameModes/LyraCloneExperienceManagerComponent.h"
#include "LyraClone/GameModes/LyraCloneGameMode.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerState)

PRAGMA_DISABLE_OPTIMIZATION
ALyraClonePlayerState::ALyraClonePlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraCloneAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}
PRAGMA_ENABLE_OPTIMIZATION

void ALyraClonePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	ULyraCloneExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceComponent);

	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraClonePlayerState::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{
	if (ALyraCloneGameMode* LyraCloneGameMode = GetWorld()->GetAuthGameMode<ALyraCloneGameMode>())
	{
		const ULyraClonePawnData* NewPawnData = LyraCloneGameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);		
	}
}

void ALyraClonePlayerState::SetPawnData(const ULyraClonePawnData* InPawnData)
{
	check(InPawnData);

	check(!PawnData);

	PawnData = InPawnData;
}

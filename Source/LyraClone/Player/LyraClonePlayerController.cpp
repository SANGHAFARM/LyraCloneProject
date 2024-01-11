#include "LyraClonePlayerController.h"

#include "LyraClone/Camera/LyraClonePlayerCameraManager.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "LyraClone/Player/LyraClonePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerController)

ALyraClonePlayerController::ALyraClonePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraClonePlayerCameraManager::StaticClass();
}

void ALyraClonePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ULyraCloneAbilitySystemComponent* LyraCloneASC = GetLyraCloneAbilitySystemComponent())
	{
		LyraCloneASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ALyraClonePlayerState* ALyraClonePlayerController::GetLyraClonePlayerState() const
{
	return CastChecked<ALyraClonePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ULyraCloneAbilitySystemComponent* ALyraClonePlayerController::GetLyraCloneAbilitySystemComponent() const
{
	const ALyraClonePlayerState* LyraClonePS = GetLyraClonePlayerState();

	return (LyraClonePS ? LyraClonePS->GetLyraCloneAbilitySystemComponent() : nullptr);
}

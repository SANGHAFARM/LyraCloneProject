#include "LyraCloneCharacter.h"

#include "LyraClonePawnExtensionComponent.h"
#include "LyraClone/Camera/LyraCloneCameraComponent.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCharacter)

ALyraCloneCharacter::ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	PawnExtComponent = CreateDefaultSubobject<ULyraClonePawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	CameraComponent = CreateDefaultSubobject<ULyraCloneCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 0.0f));
}

void ALyraCloneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

UAbilitySystemComponent* ALyraCloneCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetLyraCloneAbilitySystemComponent();
}

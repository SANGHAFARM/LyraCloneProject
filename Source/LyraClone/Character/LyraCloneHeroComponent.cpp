#include "LyraCloneHeroComponent.h"

#include "LyraClonePawnData.h"
#include "LyraClonePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EnhancedInputSubsystems.h"
#include "LyraClone/LyraCloneLogChannels.h"
#include "LyraClone/LyraCloneGameplayTags.h"
#include "LyraClone/Player/LyraClonePlayerState.h"
#include "LyraClone/Player/LyraClonePlayerController.h"
#include "LyraClone/Camera/LyraCloneCameraComponent.h"
#include "LyraClone/Input/LyraCloneInputComponent.h"
#include "LyraClone/Input/LyraCloneMappableConfigPair.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "PlayerMappableInputConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneHeroComponent)

const FName ULyraCloneHeroComponent::NAME_ActorFeatureName("Hero");
const FName ULyraCloneHeroComponent::NAME_BindInputsNow("BindInputsNow");

ULyraCloneHeroComponent::ULyraCloneHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraCloneHeroComponent::OnRegister()
{
	Super::OnRegister();
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogLyraClone, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	RegisterInitStateFeature();
}

void ULyraCloneHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	BindOnActorInitStateChanged(ULyraClonePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);
	ensure(TryToChangeInitState(FLyraCloneGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void ULyraCloneHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void ULyraCloneHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	if (Params.FeatureName == ULyraClonePawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraCloneHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ALyraClonePlayerState* PS = GetPlayerState<ALyraClonePlayerState>();
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!PS)
		{
			return false;
		}
		return true;
	}
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return PS && Manager->HasFeatureReachedInitState(Pawn,
			ULyraClonePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULyraCloneHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();

	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraClonePlayerState* LyraClonePS = GetPlayerState<ALyraClonePlayerState>();
		if (!ensure(Pawn && LyraClonePS))
		{
			return;
		}		

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULyraClonePawnData* PawnData = nullptr;

		if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>();

			// DataInitialized 단계까지 오면, Pawn이 Controller에 Possess되어 준비된 상태,
			// - InitAbilityActorInfo 호출로 AvatarActor 재설정이 필요.
			PawnExtComp->InitializeAbilitySystem(LyraClonePS->GetLyraCloneAbilitySystemComponent(), LyraClonePS);
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCloneCharacter에 Attach된 CameraComponent를 찾음
			if (ULyraCloneCameraComponent* CameraComponent = ULyraCloneCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		if (ALyraClonePlayerController* LyraClonePC = GetController<ALyraClonePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void ULyraCloneHeroComponent::CheckDefaultInitialization()
{
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable,
	InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

void ULyraCloneHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();

	if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraClonePawnData* PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>())
		{
			if (const ULyraCloneInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FLyraCloneGameplayTags& GameplayTags = FLyraCloneGameplayTags::Get();
				for (const FLyraCloneMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}

				ULyraCloneInputComponent* LyraCloneIC = CastChecked<ULyraCloneInputComponent>(PlayerInputComponent);
				{
					TArray<uint32> BindHandles;
					LyraCloneIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
					LyraCloneIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					LyraCloneIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

TSubclassOf<ULyraCloneCameraMode> ULyraCloneHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return nullptr;
	}

	if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraClonePawnData* PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void ULyraCloneHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}

		bool bLogging = true;
		if (bLogging)
		{
			UE_LOG(LogLyraClone, Log, TEXT("Input_Move[X=%.2f][Y=%.2f]"), Value.X, Value.Y);
		}
	}
}

void ULyraCloneHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}

void ULyraCloneHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraCloneAbilitySystemComponent* LyraCloneASC = PawnExtComp->GetLyraCloneAbilitySystemComponent())
			{
				LyraCloneASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void ULyraCloneHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraClonePawnExtensionComponent* PawnExtComp =
			ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraCloneAbilitySystemComponent* LyraCloneASC = PawnExtComp->GetLyraCloneAbilitySystemComponent())
			{
				LyraCloneASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}

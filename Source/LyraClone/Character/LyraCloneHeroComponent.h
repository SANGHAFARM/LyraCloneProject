#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LyraCloneHeroComponent.generated.h"

struct FInputActionValue;
struct FLyraCloneMappableConfigPair;
class ULyraCloneCameraMode;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class ULyraCloneHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULyraCloneHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;
	static const FName NAME_BindInputsNow;

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void CheckDefaultInitialization() override;
	
	TSubclassOf<ULyraCloneCameraMode> DetermineCameraMode() const;

	UPROPERTY(EditAnywhere)
	TArray<FLyraCloneMappableConfigPair> DefaultInputConfigs;

	UPROPERTY(EditAnywhere)
	TArray<FLyraCloneMappableConfigPair> DefaultConfigs;

	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	
};
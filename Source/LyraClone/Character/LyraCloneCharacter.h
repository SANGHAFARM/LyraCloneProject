#pragma once

#include "GameFramework/Character.h"

#include "LyraCloneCharacter.generated.h"

class ULyraClonePawnExtensionComponent;
class ULyraCloneCameraComponent;

UCLASS()
class ALyraCloneCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LyraCloneCharacter")
	TObjectPtr<ULyraClonePawnExtensionComponent> PawnExtComponent;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LyraClone|Character")
	TObjectPtr<ULyraCloneCameraComponent> CameraComponent;
};
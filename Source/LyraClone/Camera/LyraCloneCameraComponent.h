#pragma once

#include "Camera/CameraComponent.h"
#include "LyraCloneCameraComponent.generated.h"

class ULyraCloneCameraModeStack;
class ULyraCloneCameraMode;

template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCloneCameraMode>, FLyraCloneCameraModeDelegate);

UCLASS()
class ULyraCloneCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	ULyraCloneCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() final;

	UPROPERTY()
	TObjectPtr<ULyraCloneCameraModeStack> CameraModeStack;

	FLyraCloneCameraModeDelegate DetermineCameraModeDelegate;

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	void UpdateCameraModes();

	UFUNCTION(BlueprintPure, Category = "Lyra|Camera")
	static ULyraCloneCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraCloneCameraComponent>() : nullptr); }

	AActor* GetTargetActor() const { return GetOwner(); }

};

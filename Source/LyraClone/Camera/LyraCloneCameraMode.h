#pragma once

#include "LyraCloneCameraMode.generated.h"

class ULyraCloneCameraComponent;


UENUM(BlueprintType)
enum class ELyraCloneCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

struct FLyraCloneCameraModeView
{
	FLyraCloneCameraModeView();

	void Blend(const FLyraCloneCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UCLASS(Abstract, NotBlueprintable)
class ULyraCloneCameraMode : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	float BlendAlpha;

	float BlendWeight;

	void UpdateCameraMode(float DeltaTime);

	virtual void UpdateView(float DeltaTime);

	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	ULyraCloneCameraComponent* GetLyraCloneCameraComponent() const;
	AActor* GetTargetActor() const;

	FLyraCloneCameraModeView View;

	void UpdateBlending(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	ELyraCloneCameraModeBlendFunction BlendFunction;

};

UCLASS()
class ULyraCloneCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	ULyraCloneCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraCloneCameraMode* GetCameraModeInstance(TSubclassOf<ULyraCloneCameraMode>& CameraModeClass);

	void PushCameraMode(TSubclassOf<ULyraCloneCameraMode>& CameraModeClass);

	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneCameraMode>> CameraModeStack;

	void EvaluateStack(float DeltaTime, FLyraCloneCameraModeView& OutCameraModeView);

	void UpdateStack(float DeltaTime);

	void BlendStack(FLyraCloneCameraModeView& OutCameraModeView) const;
};

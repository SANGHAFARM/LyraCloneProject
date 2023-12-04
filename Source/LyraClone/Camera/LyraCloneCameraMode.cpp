#include "LyraCloneCameraMode.h"
#include "LyraCloneCameraComponent.h"
#include "LyraClonePlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCameraMode)

FLyraCloneCameraModeView::FLyraCloneCameraModeView() : Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(LyraClone_CAMERA_DEFAULT_FOV)
{
}

void FLyraCloneCameraModeView::Blend(const FLyraCloneCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

ULyraCloneCameraMode::ULyraCloneCameraMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FieldOfView = LyraClone_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LyraClone_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LyraClone_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = ELyraCloneCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void ULyraCloneCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);

	UpdateBlending(DeltaTime);
}

void ULyraCloneCameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

FVector ULyraCloneCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator ULyraCloneCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

ULyraCloneCameraComponent* ULyraCloneCameraMode::GetLyraCloneCameraComponent() const
{
	return CastChecked<ULyraCloneCameraComponent>(GetOuter());
}

AActor* ULyraCloneCameraMode::GetTargetActor() const
{
	const ULyraCloneCameraComponent* LyraCloneCameraComponent = GetLyraCloneCameraComponent();

	return LyraCloneCameraComponent->GetTargetActor();
}

void ULyraCloneCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case ELyraCloneCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ELyraCloneCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ELyraCloneCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ELyraCloneCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

ULyraCloneCameraModeStack::ULyraCloneCameraModeStack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

ULyraCloneCameraMode* ULyraCloneCameraModeStack::GetCameraModeInstance(TSubclassOf<ULyraCloneCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	for (ULyraCloneCameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	ULyraCloneCameraMode* NewCameraMode = NewObject<ULyraCloneCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void ULyraCloneCameraModeStack::PushCameraMode(TSubclassOf<ULyraCloneCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	ULyraCloneCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);

	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;
	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void ULyraCloneCameraModeStack::EvaluateStack(float DeltaTime, FLyraCloneCameraModeView& OutCameraModeView)
{
	UpdateStack(DeltaTime);

	BlendStack(OutCameraModeView);
}

void ULyraCloneCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();

	if (StackSize <= 0)
		return;

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		ULyraCloneCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void ULyraCloneCameraModeStack::BlendStack(FLyraCloneCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	const ULyraCloneCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	OutCameraModeView = CameraMode->View;

	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}

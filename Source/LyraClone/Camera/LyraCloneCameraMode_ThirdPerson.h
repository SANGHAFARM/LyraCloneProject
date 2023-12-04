#pragma once

#include "LyraCloneCameraMode.h"
#include "LyraCloneCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS(Abstract, Blueprintable)
class ULyraCloneCameraMode_ThirdPerson : public ULyraCloneCameraMode
{
	GENERATED_BODY()

public:
	ULyraCloneCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "LyraCloneAnimInstance.generated.h"

UCLASS()
class ULyraCloneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULyraCloneAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
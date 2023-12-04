#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraClonePawnData.generated.h"

class ULyraCloneCameraMode;
class ULyraCloneInputConfig;

UCLASS(BlueprintType)
class ULyraClonePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraClonePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Camera")
	TSubclassOf<ULyraCloneCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|InputConfig")
	TObjectPtr<ULyraCloneInputConfig> InputConfig;
};
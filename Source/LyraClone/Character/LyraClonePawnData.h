#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraClonePawnData.generated.h"

class ULyraCloneCameraMode;
class ULyraCloneInputConfig;
class ULyraCloneAbilitySet;

UCLASS(BlueprintType)
class ULyraClonePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraClonePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Pawn")
	TSubclassOf<APawn> PawnClass;

	/** Camera Mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Camera")
	TSubclassOf<ULyraCloneCameraMode> DefaultCameraMode;

	/** Input configuration used by player controlled pawns to create input mappings and bind input actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|InputConfig")
	TObjectPtr<ULyraCloneInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Abilities")
	TArray<TObjectPtr<ULyraCloneAbilitySet>> AbilitySets;
};
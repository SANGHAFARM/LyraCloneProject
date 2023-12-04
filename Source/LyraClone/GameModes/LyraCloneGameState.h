#pragma once

#include "GameFramework/GameStateBase.h"
#include "LyraCloneGameState.generated.h"

class ULyraCloneExperienceManagerComponent;

UCLASS()
class ALyraCloneGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ALyraCloneGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TObjectPtr<ULyraCloneExperienceManagerComponent> ExperienceManagerComponent;
};
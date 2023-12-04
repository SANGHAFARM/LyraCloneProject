#include "LyraCloneGameState.h"
#include "LyraCloneExperienceManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameState)

PRAGMA_DISABLE_OPTIMIZATION
ALyraCloneGameState::ALyraCloneGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<ULyraCloneExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
PRAGMA_ENABLE_OPTIMIZATION

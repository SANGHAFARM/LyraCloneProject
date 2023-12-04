#include "LyraCloneGameInstance.h"
#include "LyraClone/LyraCloneGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

ULyraCloneGameInstance::ULyraCloneGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraCloneGameInstance::Init()
{
	Super::Init();

	// Register our custom init states
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		const FLyraCloneGameplayTags& GameplayTags = FLyraCloneGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void ULyraCloneGameInstance::Shutdown()
{
	Super::Shutdown();
}

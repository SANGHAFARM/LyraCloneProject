#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LyraClonePawnExtensionComponent.generated.h"

class ULyraClonePawnData;
class ULyraCloneAbilitySystemComponent;

UCLASS()
class ULyraClonePawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULyraClonePawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	static const FName NAME_ActorFeatureName;

	virtual void OnRegister() final;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	static ULyraClonePawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraClonePawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULyraClonePawnData* InPawnData);
	void SetupPlayerInputComponent();
	void InitializeAbilitySystem(ULyraCloneAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();

	UPROPERTY(EditInstanceOnly, Category = "LyraClone|Pawn")
	TObjectPtr<const ULyraClonePawnData> PawnData;

	UPROPERTY()
	TObjectPtr<ULyraCloneAbilitySystemComponent> AbilitySystemComponent;
};
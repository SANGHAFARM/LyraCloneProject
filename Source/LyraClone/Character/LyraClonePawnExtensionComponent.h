#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LyraClonePawnExtensionComponent.generated.h"

class ULyraClonePawnData;

UCLASS()
class ULyraClonePawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULyraClonePawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnRegister() final;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static const FName NAME_ActorFeatureName;

	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	void SetPawnData(const ULyraClonePawnData* InPawnData);

	UPROPERTY(EditInstanceOnly, Category = "LyraClone|Pawn")
	TObjectPtr<const ULyraClonePawnData> PawnData;

	void SetupPlayerInputComponent();

	static ULyraClonePawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraClonePawnExtensionComponent>() : nullptr); }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

};
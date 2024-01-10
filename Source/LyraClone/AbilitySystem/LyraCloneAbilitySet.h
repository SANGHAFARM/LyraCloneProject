#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "LyraCloneAbilitySet.generated.h"

class ULyraCloneGameplayAbility;
class ULyraCloneAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FLyraCloneAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	/** 허용된 GameplayAbility */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULyraCloneGameplayAbility> Ability = nullptr;

	/** Ability의 허용 조건 (Level) */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	/** Input 처리를 위한 GameplayTag */	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

};

USTRUCT(BlueprintType)
struct FLyraCloneAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	void TakeFromAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC);
};

/** Gameplay Ability를 좀 더 쉽게 관리하기 위한 Set */
UCLASS(BlueprintType)
class ULyraCloneAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraCloneAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FLyraCloneAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	void GiveToAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC, FLyraCloneAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);
};
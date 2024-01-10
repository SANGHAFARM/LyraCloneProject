#include "LyraCloneAbilitySet.h"

#include "LyraCloneAbilitySystemComponent.h"
#include "Abilities/LyraCloneGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAbilitySet)

ULyraCloneAbilitySet::ULyraCloneAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraCloneAbilitySet::GiveToAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC, FLyraCloneAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(LyraCloneASC);

	if (!LyraCloneASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); AbilityIndex++)
	{
		const FLyraCloneAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		ULyraCloneGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<ULyraCloneGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = LyraCloneASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}

void FLyraCloneAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FLyraCloneAbilitySet_GrantedHandles::TakeFromAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC)
{
	if (!LyraCloneASC->IsOwnerActorAuthoritative())
	{
		return;
	}
	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			LyraCloneASC->ClearAbility(Handle);
		}
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * FLyraGameplayTags
 *
 *	Singleton containing native gameplay tags.
 */
struct FLyraCloneGameplayTags
{
public:

	static const FLyraCloneGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

	/** Actor/component has initially spawned and can be extended */
	FGameplayTag InitState_Spawned;

	/** All required data has been loaded/replicated and is ready for initialization */
	FGameplayTag InitState_DataAvailable;

	/** The available data has been initialized for this actor/component, but it is not ready for full gameplay */
	FGameplayTag InitState_DataInitialized;

	/** The actor/component is fully ready for active gameplay */
	FGameplayTag InitState_GameplayReady;

	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;

private:
	static FLyraCloneGameplayTags GameplayTags;
};

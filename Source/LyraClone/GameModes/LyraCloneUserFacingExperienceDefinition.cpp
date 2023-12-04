// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraCloneUserFacingExperienceDefinition.h"
#include "CommonSessionSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneUserFacingExperienceDefinition)

UCommonSession_HostSessionRequest* ULyraCloneUserFacingExperienceDefinition::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}

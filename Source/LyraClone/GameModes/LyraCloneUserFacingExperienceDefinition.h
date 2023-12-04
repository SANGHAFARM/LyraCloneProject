// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneUserFacingExperienceDefinition.generated.h"

class UCommonSession_HostSessionRequest;

UCLASS(BlueprintType)
class LYRACLONE_API ULyraCloneUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "LyraCloneExperienceDefinition"))
	FPrimaryAssetId ExperienceID;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;

};


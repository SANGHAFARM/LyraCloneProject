#pragma once

#include "LyraCloneWeaponInstance.h"

#include "LyraCloneRangedWeaponInstance.generated.h"

UCLASS()
class ULyraCloneRangedWeaponInstance : public ULyraCloneWeaponInstance
{
	GENERATED_BODY()

public:
	ULyraCloneRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;

};
#pragma once

#include "CoreMinimal.h"
#include "LyraClone/Equipment/LyraCloneGameplayAbility_FromEquipment.h"

#include "LyraCloneGameplayAbility_RangedWeapon.generated.h"

class ULyraCloneRangedWeaponInstance;

UENUM(BlueprintType)
enum class ELyraCloneAbilityTargetingSource : uint8
{
	CameraTowardsFocus,
};

UCLASS()
class ULyraCloneGameplayAbility_RangedWeapon : public ULyraCloneGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		ULyraCloneRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput() : StartTrace(ForceInitToZero), EndAim(ForceInitToZero), AimDir(ForceInitToZero)
		{}
	};

	ULyraCloneGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void PerformLocalTargeting(TArray<FHitResult>& OutHits);

	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;

	int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const;

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;

	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	ULyraCloneEquipmentInstance* GetAssociatedEquipment() const;
	ULyraCloneRangedWeaponInstance* GetWeaponInstance();
	FVector GetWeaponTargetingSourceLocation() const;
	FTransform GetTargetingTransform(APawn* SourcePawn, ELyraCloneAbilityTargetingSource Source);
};
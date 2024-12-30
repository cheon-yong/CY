// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CYGA_AttackWithMonatage.h"
#include "CYGA_ProjectileAttack.generated.h"

class UCYAnimNotify_AttackHitCheck;
class ACYProjectile;

/**
 * 
 */
UCLASS()
class CY_API UCYGA_ProjectileAttack : public UCYGA_AttackWithMonatage
{
	GENERATED_BODY()
	
public:
	UCYGA_ProjectileAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void BindNotify() override;

	UFUNCTION()
	void OnNotifyBegin();

	UFUNCTION()
	FVector GetSpawnLocation();

	UFUNCTION()
	FRotator GetSpawnRotator();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ACYProjectile> ProjectileClass;

	UPROPERTY()
	FVector SpawnLocation;

	UPROPERTY()
	FRotator SpawnRotator;

	TObjectPtr<UCYAnimNotify_AttackHitCheck> AnimNotify_HitCheck;
};

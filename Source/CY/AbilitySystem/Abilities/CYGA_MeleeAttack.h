// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CYGA_AttackWithMonatage.h"
#include "CYGA_MeleeAttack.generated.h"

class UCYAnimNotify_AttackHitCheck;
class ACYTA_Trace;
/**
 * 
 */
UCLASS()
class CY_API UCYGA_MeleeAttack : public UCYGA_AttackWithMonatage
{
	GENERATED_BODY()
	
public:
	UCYGA_MeleeAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void BindNotify() override;

	UFUNCTION()
	void OnNotifyBegin();

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
protected:

	TObjectPtr<UCYAnimNotify_AttackHitCheck> AnimNotify_HitCheck;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<ACYTA_Trace> TargetActorClass;
};

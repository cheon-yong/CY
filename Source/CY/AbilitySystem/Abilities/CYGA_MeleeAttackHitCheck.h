// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CYGameplayAbility.h"
#include "CYGA_MeleeAttackHitCheck.generated.h"

class UGameplayEffect;
class ACYTA_Trace;
/**
 * 
 */
UCLASS()
class CY_API UCYGA_MeleeAttackHitCheck : public UCYGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCYGA_MeleeAttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<ACYTA_Trace> TargetActorClass;
};

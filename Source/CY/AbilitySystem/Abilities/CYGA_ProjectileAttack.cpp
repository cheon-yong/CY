// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/CYGA_ProjectileAttack.h"
#include "Character/CYCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCYGA_ProjectileAttack::UCYGA_ProjectileAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCYGA_ProjectileAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACYCharacter* CYCharacter = CastChecked<ACYCharacter>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UCYGA_ProjectileAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UCYGA_ProjectileAttack::OnInterruptedCallback);

	PlayAttackTask->ReadyForActivation();
}

void UCYGA_ProjectileAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCYGA_ProjectileAttack::OnCompleteCallback()
{
}

void UCYGA_ProjectileAttack::OnInterruptedCallback()
{
}

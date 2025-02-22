// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CYGA_AttackWithMonatage.h"
#include "Character/CYCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCYGA_AttackWithMonatage::UCYGA_AttackWithMonatage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCYGA_AttackWithMonatage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACYCharacter* CYCharacter = CastChecked<ACYCharacter>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UCYGA_AttackWithMonatage::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UCYGA_AttackWithMonatage::OnInterruptedCallback);
	PlayAttackTask->OnCancelled.AddDynamic(this, &UCYGA_AttackWithMonatage::OnCancelledCallback);
	PlayAttackTask->OnBlendOut.AddDynamic(this, &UCYGA_AttackWithMonatage::OnCancelledCallback);

	// Bind Something
	BindNotify();

	PlayAttackTask->ReadyForActivation();
}

void UCYGA_AttackWithMonatage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACYCharacter* CYCharacter = CastChecked<ACYCharacter>(ActorInfo->AvatarActor.Get());
	CYCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCYGA_AttackWithMonatage::BindNotify()
{
}

void UCYGA_AttackWithMonatage::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UCYGA_AttackWithMonatage::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UCYGA_AttackWithMonatage::OnCancelledCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

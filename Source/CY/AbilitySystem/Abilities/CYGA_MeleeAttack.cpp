// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CYGA_MeleeAttack.h"
#include "Animation/CYAnimNotify_AttackHitCheck.h"
#include "AbilitySystem/AbilityTask/CYAT_Trace.h"
#include "AbilitySystem/TargetActor/CYTA_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"

UCYGA_MeleeAttack::UCYGA_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCYGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCYGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AnimNotify_HitCheck->OnNotified.RemoveAll(this);

	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
void UCYGA_MeleeAttack::BindNotify()
{
	const auto NotifyEvents = AttackMontage->Notifies;
	for (auto NotifyEvent : NotifyEvents)
	{
		if (const auto HitCheckNotify = Cast<UCYAnimNotify_AttackHitCheck>(NotifyEvent.Notify))
		{
			HitCheckNotify->OnNotified.AddDynamic(this, &ThisClass::OnNotifyBegin);
			AnimNotify_HitCheck = HitCheckNotify;
			break;
		}
	}
}


void UCYGA_MeleeAttack::OnNotifyBegin()
{
	if (!CurrentActorInfo->AvatarActor->HasAuthority())
	{
		return;
	}

	UCYAT_Trace* AttackTraceTask = UCYAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UCYGA_MeleeAttack::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UCYGA_MeleeAttack::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (true)
	{
		if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
		{
			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
			if (!SourceASC || !TargetASC)
			{
				UE_LOG(LogTemp, Error, TEXT("ASC not found!"));
				return;
			}

			const UCYAttributeSet* SourceAttribute = SourceASC->GetSet<UCYAttributeSet>();

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, 1);
			if (EffectSpecHandle.IsValid())
			{
				//EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());			
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

				FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
				CueContextHandle.AddHitResult(HitResult);
				FGameplayCueParameters CueParam;
				CueParam.EffectContext = CueContextHandle;

				//TargetASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
			}
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

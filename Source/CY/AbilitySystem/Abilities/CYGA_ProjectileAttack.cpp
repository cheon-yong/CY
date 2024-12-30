// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/CYGA_ProjectileAttack.h"
#include "Character/CYCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Actor/CYProjectile.h"
#include "Animation/CYAnimNotify_AttackHitCheck.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "Components/BoxComponent.h"


UCYGA_ProjectileAttack::UCYGA_ProjectileAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCYGA_ProjectileAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UCYGA_ProjectileAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AnimNotify_HitCheck->OnNotified.RemoveAll(this);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCYGA_ProjectileAttack::BindNotify()
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

void UCYGA_ProjectileAttack::OnNotifyBegin()
{
	SpawnLocation = GetSpawnLocation();
	SpawnRotator = GetSpawnRotator();

	if (!CurrentActorInfo->AvatarActor->HasAuthority())
	{
		return;
	}

	ACYCharacter* CYCharacter = GetCYCharacterFromActorInfo();
	if (CYCharacter == nullptr)
		return;

	FTransform SpawnTransform(SpawnRotator, SpawnLocation);
	if (ACYProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACYProjectile>(ProjectileClass, SpawnTransform))
	{
		// Ignore each other
		CYCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
		Projectile->GetBoxComponent()->IgnoreActorWhenMoving(CYCharacter, true);
		Projectile->GetStaticMesh()->IgnoreActorWhenMoving(CYCharacter, true);

		// Set Effect
		Projectile->SetGameplayEffect(AttackDamageEffect);

		// Init Projectile
		Projectile->InitProjectile(CYCharacter);

		// Finish Spawn
		Projectile->FinishSpawning(SpawnTransform);
		Projectile->BeginReplication();
	}
}

FVector UCYGA_ProjectileAttack::GetSpawnLocation()
{
	ACYCharacter* CYCharacter = GetCYCharacterFromActorInfo();
	if (CYCharacter == nullptr)
		return FVector::ZeroVector;

	FVector ActorLocation = CYCharacter->GetActorLocation();
	FVector ForwardVector = CYCharacter->GetActorForwardVector();
	FVector UpVector = CYCharacter->GetActorUpVector();

	return ActorLocation + (ForwardVector * 5.0f) + (UpVector * 25.0f);
}

FRotator UCYGA_ProjectileAttack::GetSpawnRotator()
{
	AController* Controller = GetControllerFromActorInfo();
	if (Controller == nullptr)
		return FRotator::ZeroRotator;

	FRotator ControllerRotator = Controller->GetControlRotation();
	return ControllerRotator;
}

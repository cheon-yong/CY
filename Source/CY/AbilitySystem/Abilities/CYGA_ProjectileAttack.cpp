// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/CYGA_ProjectileAttack.h"
#include "Character/CYCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Actor/CYProjectile.h"

UCYGA_ProjectileAttack::UCYGA_ProjectileAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCYGA_ProjectileAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SpawnLocation = GetSpawnLocation();
	SpawnRotator = GetSpawnRotator();

	// In server
	if (HasAuthority(&ActivationInfo))
	{
		ACYCharacter* CYCharacter = GetCYCharacterFromActorInfo();
		if (CYCharacter == nullptr)
			return;

		FTransform SpawnTransform(SpawnRotator, SpawnLocation);
		if (ACYProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACYProjectile>(ProjectileClass, SpawnTransform))
		{
			// Ignore each other
			CYCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
			Projectile->GetStaticMesh()->IgnoreActorWhenMoving(CYCharacter, true);

			// Set Instigator
			Projectile->SetInstigator(CYCharacter);

			Projectile->GetProjectileMovement()->bRotationFollowsVelocity = true;

			// Finish Spawn
			Projectile->FinishSpawning(SpawnTransform);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
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

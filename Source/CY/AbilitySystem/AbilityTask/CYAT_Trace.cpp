// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/CYAT_Trace.h"
#include "AbilitySystem/TargetActor/CYTA_Trace.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"

UCYAT_Trace::UCYAT_Trace()
{
}

UCYAT_Trace* UCYAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class UObject> TargetActorClass)
{
	UCYAT_Trace* NewTask = NewAbilityTask<UCYAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UCYAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCYAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UCYAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACYTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UCYAT_Trace::OnTargetDataReadyCallback);
	}
}

void UCYAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UCYAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
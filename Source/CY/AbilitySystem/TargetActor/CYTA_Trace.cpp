// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TargetActor/CYTA_Trace.h"
#include "AbilitySystem/Abilities/CYGameplayAbility.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "Character/CYCharacter.h"
#include "Components/CapsuleComponent.h"
#include <Physics/CYCollision.h>

ACYTA_Trace::ACYTA_Trace()
{
}

void ACYTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACYTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ACYTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UCYAttributeSet* AttributeSet = ASC->GetSet<UCYAttributeSet>();
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("UCYAttributeSet not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult OutHitResult;
	// TODO : 수정할 것
	// const float AttackRange = AttributeSet->GetAttackRange();
	// const float AttackRadius = AttributeSet->GetAttackRadius();
	const float AttackRange = 200.f;
	const float AttackRadius = 50.f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ACYTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_CYACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}

#endif

	return DataHandle;
}

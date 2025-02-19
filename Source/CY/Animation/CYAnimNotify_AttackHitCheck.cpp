// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CYAnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UCYAnimNotify_AttackHitCheck::UCYAnimNotify_AttackHitCheck()
{
	
}

FString UCYAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("AttackHitCheck");
}

void UCYAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
		if (OwnerPawn && OwnerPawn->IsLocallyControlled())
		{
			OnNotified.Broadcast();
			FGameplayEventData PayloadData;
			PayloadData.Instigator = OwnerPawn;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerPawn, TriggerGameplayTag, PayloadData);
		}
	}
}

//서버 -> A(서버) B(클라)
//
//클라 -> A(서버) B(클라)
//
//클라에서 B가 액션을 하면 클라 B의 AnimNotify 호출
//서버에서 B도 액션을 하면서 AnimNotify 호출 그러나 여기서 Replication 발생
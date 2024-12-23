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
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}

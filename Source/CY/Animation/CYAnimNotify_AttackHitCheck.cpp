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

//���� -> A(����) B(Ŭ��)
//
//Ŭ�� -> A(����) B(Ŭ��)
//
//Ŭ�󿡼� B�� �׼��� �ϸ� Ŭ�� B�� AnimNotify ȣ��
//�������� B�� �׼��� �ϸ鼭 AnimNotify ȣ�� �׷��� ���⼭ Replication �߻�
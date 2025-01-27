// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "CYAnimNotify_AttackHitCheck.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifiedSignature);

/**
 * 
 */
UCLASS()
class CY_API UCYAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UCYAnimNotify_AttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnNotifiedSignature OnNotified;

protected:
	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTag TriggerGameplayTag;
};

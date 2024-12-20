// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CYGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CY_API UCYGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag InputTag;
};

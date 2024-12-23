// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CYGameplayAbility.generated.h"


UENUM(BlueprintType)
enum class ECYAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/**
 * 
 */
UCLASS()
class CY_API UCYGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = InputTag))
	FGameplayTag InputTag;

	ECYAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy ; }


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CY|Ability Activation")
	ECYAbilityActivationPolicy ActivationPolicy;
};

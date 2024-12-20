// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CYPlayerState.generated.h"


class UCYAbilitySystemComponent;
class UCYAttributeSet;
/**
 * 
 */
UCLASS()
class CY_API ACYPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACYPlayerState();

	virtual UCYAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UCYAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UCYAttributeSet> AttributeSet;
};

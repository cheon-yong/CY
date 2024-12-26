// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"

#include "CYUserWidget.generated.h"

class UCYAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class CY_API UCYUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);

	UFUNCTION()
	virtual void SetAbilitySystemComponentByASC(UCYAbilitySystemComponent* OwnerASC);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const { return ASC; }

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UCYAbilitySystemComponent> ASC;
};

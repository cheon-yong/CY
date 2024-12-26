// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"

#include "CYUserWidget.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class CY_API UCYUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

};

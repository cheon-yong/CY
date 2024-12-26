// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CYUserWidget.h"

#include "AbilitySystemBlueprintLibrary.h"

void UCYUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UCYUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CYUserWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CYCharacter.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"

void UCYUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		if (ACYCharacter* CYCharacter = Cast<ACYCharacter>(InOwner))
		{
			UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
			UCYAbilitySystemComponent* OwnerCYASC = Cast<UCYAbilitySystemComponent>(OwnerASC);
			if (OwnerCYASC != nullptr)
				ASC = OwnerCYASC;
			else
				CYCharacter->OnInitASC.AddDynamic(this, &ThisClass::SetAbilitySystemComponentByASC);
		}
	}
}

void UCYUserWidget::SetAbilitySystemComponentByASC(UCYAbilitySystemComponent* OwnerASC)
{
	if (OwnerASC == nullptr)
		return;

	ASC = OwnerASC;
}

UAbilitySystemComponent* UCYUserWidget::GetAbilitySystemComponent() const
{
	return GetCYAbilitySystemComponent();
}

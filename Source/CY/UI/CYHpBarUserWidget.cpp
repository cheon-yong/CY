// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CYHpBarUserWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCYHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UCYAttributeSet::GetHealthAttribute()).AddUObject(this, &UCYHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UCYAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UCYHpBarUserWidget::OnMaxHealthChanged);
		
		PbHpBar->SetFillColorAndOpacity(HealthColor);

		const UCYAttributeSet* CurrentAttributeSet = ASC->GetSet<UCYAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UCYHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UCYHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UCYHpBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void UCYHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CYUserWidget.h"
#include "GameplayEffectTypes.h"

#include "CYHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CY_API UCYHpBarUserWidget : public UCYUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
	virtual void SetAbilitySystemComponentByASC(UCYAbilitySystemComponent* OwnerASC) override;


	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHpBar();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};

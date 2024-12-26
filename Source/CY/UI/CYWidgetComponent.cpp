// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CYWidgetComponent.h"
#include "UI/CYUserWidget.h"

void UCYWidgetComponent::InitWidget()
{
	Super::InitWidget();

	if (UCYUserWidget* CYUserWidget = Cast<UCYUserWidget>(GetWidget()))
	{
		CYUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}

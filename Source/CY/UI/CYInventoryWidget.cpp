// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CYInventoryWidget.h"
#include "GameFramework/PlayerController.h"
#include "Character/CYCharacter.h"
#include "Inventory/CYInventoryComponent.h"
#include "Inventory/CYItemInstance.h"

UCYInventoryWidget::UCYInventoryWidget()
{
}

void UCYInventoryWidget::GetItemsFromInventoryComponent()
{
	if (ACYCharacter* PlayerPawn = GetOwningPlayerPawn<ACYCharacter>())
	{
		if (UCYInventoryComponent* Inventory = PlayerPawn->GetComponentByClass<UCYInventoryComponent>())
		{
			ItemInstances = Inventory->GetAllItemInstances();
		}
	}
}

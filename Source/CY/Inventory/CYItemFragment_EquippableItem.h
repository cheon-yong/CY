// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/CYItemDefinition.h"
#include "CYItemFragment_EquippableItem.generated.h"

class UCYEquipmentDefinition;

/**
 * 
 */
UCLASS()
class CY_API UCYItemFragment_EquippableItem : public UCYItemFragment
{
	GENERATED_BODY()
	
public:
	/*UPROPERTY(EditAnywhere, Category = CY)
	TSubclassOf<UCYEquipmentDefinition> EquipmentDefinition;*/
};

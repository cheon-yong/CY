// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/CYItemDefinition.h"
#include "Styling/SlateBrush.h"

#include "CYItemFragment_ItemInfo.generated.h"


/**
 * 
 */
UCLASS()
class CY_API UCYItemFragment_ItemInfo : public UCYItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StackCount)
	int32 MaxCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Appearance)
	FSlateBrush IconBrush;
};

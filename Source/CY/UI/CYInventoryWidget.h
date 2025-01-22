// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CYUserWidget.h"
#include "CYInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class CY_API UCYInventoryWidget : public UCYUserWidget
{
	GENERATED_BODY()
	
public:
	UCYInventoryWidget();

	UFUNCTION(BlueprintCallable)
	void GetItems();

protected:

};

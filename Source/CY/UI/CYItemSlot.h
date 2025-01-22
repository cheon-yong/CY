// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CYUserWidget.h"
#include "CYItemSlot.generated.h"

class UTextBlock;
class UImage;
class UCYItemDefinition;

/**
 * 
 */
UCLASS()
class CY_API UCYItemSlot : public UCYUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemInfo(UCYItemDefinition* ItemDefinition);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BgImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
};

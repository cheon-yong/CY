// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CYWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class CY_API UCYWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};

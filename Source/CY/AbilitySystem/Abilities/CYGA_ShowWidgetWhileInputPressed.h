// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CYGameplayAbility.h"
#include "CYGA_ShowWidgetWhileInputPressed.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class CY_API UCYGA_ShowWidgetWhileInputPressed : public UCYGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget)
	TSubclassOf<UUserWidget> WidgetClass;
};

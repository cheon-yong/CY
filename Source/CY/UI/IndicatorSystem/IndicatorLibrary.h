// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "IndicatorLibrary.generated.h"

class AController;
class UCYIndicatorManagerComponent;
class UObject;
struct FFrame;

UCLASS()
class UIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UIndicatorLibrary();
	
	/**  */
	UFUNCTION(BlueprintCallable, Category = Indicator)
	static UCYIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller);
};

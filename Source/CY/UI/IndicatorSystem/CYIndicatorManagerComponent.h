// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"

#include "CYIndicatorManagerComponent.generated.h"

class AController;
class UIndicatorDescriptor;
class UObject;
struct FFrame;

/**
 * @class UCYIndicatorManagerComponent
 */
UCLASS(BlueprintType, Blueprintable)
class CY_API UCYIndicatorManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UCYIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UCYIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(UCYIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
};

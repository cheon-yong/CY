// Copyright Epic Games, Inc. All Rights Reserved.

#include "IndicatorLibrary.h"

#include "CYIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorLibrary)

class AController;

UIndicatorLibrary::UIndicatorLibrary()
{
}

UCYIndicatorManagerComponent* UIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return UCYIndicatorManagerComponent::GetComponent(Controller);
}


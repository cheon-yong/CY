// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/CYItemDefinition.h"
#include "CYItemFragment_SpawnableToField.generated.h"

class ACYItemActor;

/**
 * 
 */
UCLASS()
class CY_API UCYItemFragment_SpawnableToField : public UCYItemFragment
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<ACYItemActor> ActorToSpawn;
};

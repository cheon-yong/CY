// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory/CYItemDefinition.h"
#include "Actor/CYItemActor.h"

#include "CYItemFragment_EquippableItem.generated.h"

class ACYItemActor;

USTRUCT(BlueprintType)
struct FCYEquipmentActorToSpawn
{
	GENERATED_BODY()

	FCYEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<ACYItemActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS()
class CY_API UCYItemFragment_EquippableItem : public UCYItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Equipment)
	TArray<FCYEquipmentActorToSpawn> ActorsToSpawn;
};

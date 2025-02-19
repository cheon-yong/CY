// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/DataAsset.h"
#include "FieldItemData.generated.h"


class UCYItemDefinition;



USTRUCT(BlueprintType)
struct FItemToSpawn
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCYItemDefinition> ItemDef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CountToCreate;
};

/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Field Item Data", ShortTooltip = "Data asset used to define a Pawn."))
class CY_API UFieldItemData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFieldItemData();

	TArray<FItemToSpawn> GetItemsToSpawn() { return ItemsToSpawn; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<FItemToSpawn> ItemsToSpawn;
};

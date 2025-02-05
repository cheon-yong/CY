// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/Serialization/FastArraySerializer.h"
#include "CYItemEntry.generated.h"

class UCYItemInstance;
class UCYItemDefinition;
class UCYInventoryComponent;
class UCYEquipmentComponent;

USTRUCT(BlueprintType)
struct FCYItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCYItemEntry() {}

	FString GetDebugString() const;

private:
	friend FCYItemList;
	friend UCYInventoryComponent;
	friend UCYEquipmentComponent;

	UPROPERTY()
	TObjectPtr<UCYItemInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FCYItemList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCYItemList()
		: OwnerComponent(nullptr)
	{
	}

	FCYItemList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	FCYItemList(UActorComponent* InOwnerComponent, int32 InMaxSize)
		: OwnerComponent(InOwnerComponent)
		, MaxSize(InMaxSize)
	{
		Entries.SetNum(MaxSize);
	}

	int32 FindEmptyIndex();

	UCYItemInstance* GetItem(int32 Index);
	UCYItemInstance* AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass, int32 StackCount = 1);
	UCYItemInstance* AddItem(UCYItemInstance* InItemInstance);
	UCYItemInstance* AddItem(UCYItemInstance* InItemInstance, int32 Index);

	void RemoveItem(UCYItemInstance* InItemInstance);
	
	TArray<FCYItemEntry> GetItemList() { return Entries; }

	TArray<UCYItemInstance*> GetAllItemInstances() const;

private:
	UPROPERTY()
	int32 MaxSize = 0;

	// Replicated list of items
	UPROPERTY()
	TArray<FCYItemEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

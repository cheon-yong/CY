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
	int32 SlotIndex = 0;
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

		for (int i = 0; i < MaxSize; i++)
		{
			Entries[i].SlotIndex = i;
		}
	}

	int32 FindEmptyIndex();

	UCYItemInstance* GetItem(int32 Index);
	UCYItemInstance* AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass, int32 StackCount = 1);
	UCYItemInstance* AddItem(UCYItemInstance* InItemInstance, bool bAddForce = false);
	UCYItemInstance* AddItem(UCYItemInstance* InItemInstance, int32 Index);

	void RemoveItem(UCYItemInstance* InItemInstance);
	void RemoveItem(int32 Index);
	void SwapItem(int32 a, int32 b);

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

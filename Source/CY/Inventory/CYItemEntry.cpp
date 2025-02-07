// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYItemEntry.h"
#include "Inventory/CYItemDefinition.h"
#include "Inventory/CYItemInstance.h"


//////////////////////////////////////////////////////////////////////
// FCYInventoryEntry

FString FCYItemEntry::GetDebugString() const
{
	return FString();
}


//////////////////////////////////////////////////////////////////////
// FCYInventoryList

int32 FCYItemList::FindEmptyIndex()
{
	for (int i = 0; i < Entries.Num(); i++)
	{
		if (Entries[i].Instance == nullptr)
			return i;
	}

	return INDEX_NONE;
}

UCYItemInstance* FCYItemList::GetItem(int32 Index)
{	
	return Entries[Index].Instance;
}

UCYItemInstance* FCYItemList::AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass, int32 StackCount)
{
	int32 EmptyIndex = FindEmptyIndex();
	if (EmptyIndex == INDEX_NONE)
		return nullptr;

	FCYItemEntry Item;
	Item.Instance = NewObject<UCYItemInstance>();
	Item.Instance->Init(InItemDefinitionClass);
	Item.Instance->StackCount = StackCount;
	Entries[EmptyIndex] = Item;
	MarkItemDirty(Item);

	return Item.Instance;
}

UCYItemInstance* FCYItemList::AddItem(UCYItemInstance* InItemInstance)
{
	int32 EmptyIndex = FindEmptyIndex();
	if (EmptyIndex == INDEX_NONE)
		return nullptr;

	FCYItemEntry Item;
	Item.Instance = InItemInstance;
	Entries[EmptyIndex] = Item;
	MarkItemDirty(Item);
	return Item.Instance;
}

UE_DISABLE_OPTIMIZATION
UCYItemInstance* FCYItemList::AddItem(UCYItemInstance* InItemInstance, int32 Index)
{
	FCYItemEntry Item;
	Item.Instance = InItemInstance;
	Entries[Index] = Item;
	MarkItemDirty(Item);

	auto Items = GetAllItemInstances();

	return Item.Instance;
}
UE_ENABLE_OPTIMIZATION

void FCYItemList::RemoveItem(UCYItemInstance* InItemInstance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FCYItemEntry& Entry = *EntryIt;
		if (Entry.Instance == InItemInstance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UE_DISABLE_OPTIMIZATION
// TODO : 더 세련된 방법이 있을 듯
void FCYItemList::SwapItem(int32 a, int32 b)
{
	FCYItemEntry temp;

	temp.Instance = Entries[b].Instance;
	Entries[b].Instance = Entries[a].Instance;
	Entries[a].Instance = temp.Instance;

	//MarkArrayDirty();
}
UE_ENABLE_OPTIMIZATION

TArray<UCYItemInstance*> FCYItemList::GetAllItemInstances() const
{
	TArray<UCYItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FCYItemEntry& Entry : Entries)
	{
		Results.Add(Entry.Instance);
		//if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		//{
		//	
		//}
	}
	return Results;
}
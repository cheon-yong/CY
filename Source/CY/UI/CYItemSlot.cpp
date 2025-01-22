// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CYItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/CYItemDefinition.h"
#include "Inventory/CYItemFragment_ItemInfo.h"

void UCYItemSlot::SetItemInfo(UCYItemDefinition* ItemDefinition)
{
	if (UCYItemFragment_ItemInfo* ItemInfo = Cast<UCYItemFragment_ItemInfo>(ItemDefinition->FindFragmentByClass(UCYItemFragment_ItemInfo::StaticClass())))
	{

	}
}

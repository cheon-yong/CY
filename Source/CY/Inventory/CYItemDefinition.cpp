// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYItemDefinition.h"

UCYItemDefinition::UCYItemDefinition()
{
}

const TObjectPtr<UCYItemFragment> UCYItemDefinition::FindFragmentByClass(TSubclassOf<UCYItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UCYItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UCYItemFragment* UCYInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UCYItemDefinition> ItemDef, TSubclassOf<UCYItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UCYItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

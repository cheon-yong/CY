// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYItemInstance.h"

#include "Inventory/CYItemDefinition.h"
#include "Net/UnrealNetwork.h"

UCYItemInstance::UCYItemInstance()
{
}

void UCYItemInstance::Init(TSubclassOf<UCYItemDefinition> InItemDefinitionClass)
{
	ItemDefinitionClass = InItemDefinitionClass;
}

void UCYItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCYItemInstance, ItemDefinitionClass);
}

const UCYItemDefinition* UCYItemInstance::GetItemDefinition() const
{
	if (IsValid(ItemDefinitionClass))
	{
		return GetDefault<UCYItemDefinition>(ItemDefinitionClass);
	}
	return nullptr;
}

const UCYItemFragment* UCYItemInstance::FindFragmentByClass(TSubclassOf<UCYItemFragment> FragmentClass) const
{
	if ((ItemDefinitionClass != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UCYItemDefinition>(ItemDefinitionClass)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

void UCYItemInstance::OnEquipped()
{
}

void UCYItemInstance::OnUnequipped()
{
}

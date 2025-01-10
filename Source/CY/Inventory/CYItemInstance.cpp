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

TSubclassOf<UCYItemDefinition> UCYItemInstance::GetItemDefinition() const
{
	return ItemDefinitionClass;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYInventoryComponent.h"

#include "Inventory/CYItemInstance.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////
// FCYInventoryEntry

FString FCYInventoryEntry::GetDebugString() const
{
	return FString();
}


//////////////////////////////////////////////////////////////////////
// FCYInventoryList

TArray<UCYItemInstance*> FCYInventoryList::GetAllItems() const
{
	TArray<UCYItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FCYInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}


//////////////////////////////////////////////////////////////////////
// FCYInventoryComponent

// Sets default values for this component's properties
UCYInventoryComponent::UCYInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCYInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCYInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCYInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}


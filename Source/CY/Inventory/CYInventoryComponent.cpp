// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYInventoryComponent.h"

#include "Inventory/CYItemInstance.h"
#include "Inventory/CYItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

//////////////////////////////////////////////////////////////////////
// FCYInventoryComponent

// Sets default values for this component's properties
UCYInventoryComponent::UCYInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this, MaxItemCount)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	
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

	if (InventoryList.GetItemList().Num() > 0)
	{
		for (FCYItemEntry& Item : InventoryList.GetItemList())
		{
			UCYItemInstance* ItemInstance = Item.Instance;
			if (ItemInstance == nullptr)
				continue;

			const UCYItemDefinition* ItemDefinition = ItemInstance->GetItemDefinition();

			if (IsValid(ItemInstance) && IsValid(ItemDefinition))
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					0,
					FColor::Blue,
					FString::Printf(TEXT("Item : %s"), *ItemDefinition->ItemName.ToString())
				);
			}
		}
	}
}

void UCYInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UCYInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (auto ItemClass : DefaultItems)
	{
		InventoryList.AddItem(ItemClass);
	}
}

bool UCYInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FCYItemEntry& Item : InventoryList.GetItemList())
	{
		UCYItemInstance* ItemInstance = Item.Instance;

		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

UCYItemInstance* UCYInventoryComponent::GetItem(int32 Index)
{
	return InventoryList.GetItem(Index);
}

void UCYInventoryComponent::AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemDefinitionClass);
	}
}

void UCYInventoryComponent::AddItemInstance(UCYItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemInstance);
	}
}

void UCYInventoryComponent::AddItemInstanceToSlot(UCYItemInstance* InItemInstance, int32 Index)
{
	InventoryList.AddItem(InItemInstance, Index);
	if (GetOwner()->HasAuthority())
	{
		
	}
}

void UCYInventoryComponent::RemoveItem(UCYItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(InItemInstance);
	}
}

void UCYInventoryComponent::RemoveItemFromSlot(int32 Index)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(Index);
	}
}

void UCYInventoryComponent::SwapItem(int32 a, int32 b)
{
	InventoryList.SwapItem(a, b);

	auto Items = InventoryList.GetItemList();
}


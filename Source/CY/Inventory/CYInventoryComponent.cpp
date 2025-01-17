// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYInventoryComponent.h"

#include "Inventory/CYItemInstance.h"
#include "Inventory/CYItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

//////////////////////////////////////////////////////////////////////
// FCYInventoryEntry

FString FCYInventoryEntry::GetDebugString() const
{
	return FString();
}


//////////////////////////////////////////////////////////////////////
// FCYInventoryList

void FCYInventoryList::AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass)
{
	FCYInventoryEntry& Item = Entries.AddDefaulted_GetRef();
	Item.Instance = NewObject<UCYItemInstance>();
	Item.Instance->Init(InItemDefinitionClass);
	MarkItemDirty(Item);
}

void FCYInventoryList::AddItem(UCYItemInstance* InItemInstance)
{
	FCYInventoryEntry& Item = Entries.AddDefaulted_GetRef();
	Item.Instance = InItemInstance;
	MarkItemDirty(Item);
}

void FCYInventoryList::RemoveItem(UCYItemInstance* InItemInstance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FCYInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == InItemInstance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}	
}

TArray<UCYItemInstance*> FCYInventoryList::GetAllItemInstances() const
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
		for (FCYInventoryEntry& Item : InventoryList.GetItemList())
		{
			UCYItemInstance* ItemInstance = Item.Instance;
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

	for (FCYInventoryEntry& Item : InventoryList.GetItemList())
	{
		UCYItemInstance* ItemInstance = Item.Instance;

		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
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

void UCYInventoryComponent::RemoveItem(UCYItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(InItemInstance);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/IPickupable.h"

#include "GameFramework/Actor.h"
#include "Inventory/CYInventoryComponent.h"
#include "UObject/ScriptInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IPickupable)

class UActorComponent;

// Add default functionality here for any IPickupable functions that are not pure virtual.

UPickupableStatics::UPickupableStatics()
	: Super(FObjectInitializer::Get())
{
}

TScriptInterface<IPickupable> UPickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<IPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IPickupable>();
}

UE_DISABLE_OPTIMIZATION
void UPickupableStatics::AddPickupToInventory(UCYInventoryComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup)
{
	if (InventoryComponent && Pickup)
	{
		const FInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		if (PickupInventory.Instances.Num() > 0)
		{
			for (const FPickupInstance& Instance : PickupInventory.Instances)
			{
				InventoryComponent->AddItemInstance(Instance.Item);
			}
		}
		else
		{
			for (const FPickupTemplate& Template : PickupInventory.Templates)
			{
				InventoryComponent->AddItem(Template.ItemDef, Template.StackCount);
			}
		}
	}
}
UE_ENABLE_OPTIMIZATION
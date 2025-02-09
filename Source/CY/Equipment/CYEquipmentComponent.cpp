// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/CYEquipmentComponent.h"

#include "AbilitySystemGlobals.h"
#include "Engine/ActorChannel.h"
#include "Inventory/CYItemDefinition.h"
#include "Inventory/CYItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include <Inventory/CYItemFragment_EquippableItem.h>

////////////////////////////////////////////////////////////////////////
//// FCYAppliedEquipmentEntry
//
//FString FCYAppliedEquipmentEntry::GetDebugString() const
//{
//	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
//}
//
////////////////////////////////////////////////////////////////////////
//// FCYEquipmentList
//
//
//void FCYEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
//{
//	for (int32 Index : RemovedIndices)
//	{
//		const FCYAppliedEquipmentEntry& Entry = Entries[Index];
//		if (Entry.Instance != nullptr)
//		{
//			Entry.Instance->OnUnequipped();
//		}
//	}
//}
//
//void FCYEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
//{
//	for (int32 Index : AddedIndices)
//	{
//		const FCYAppliedEquipmentEntry& Entry = Entries[Index];
//		if (Entry.Instance != nullptr)
//		{
//			Entry.Instance->OnEquipped();
//		}
//	}
//}
//
//void FCYEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
//{
//}
//
//UCYItemInstance* FCYEquipmentList::AddEntry(TSubclassOf<UCYItemDefinition> EquipmentDefinition)
//{
//	UCYItemInstance* Result = nullptr;
//
//	check(EquipmentDefinition != nullptr);
//	check(OwnerComponent);
//	check(OwnerComponent->GetOwner()->HasAuthority());
//
//	const UCYItemDefinition* EquipmentCDO = GetDefault<UCYItemDefinition>(EquipmentDefinition);
//
//	TSubclassOf<UCYItemInstance> InstanceType = nullptr;
//	if (InstanceType == nullptr)
//	{
//		InstanceType = UCYItemInstance::StaticClass();
//	}
//
//	FCYAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
//	NewEntry.EquipmentDefinition = EquipmentDefinition;
//	NewEntry.Instance = NewObject<UCYItemInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
//	Result = NewEntry.Instance;
//
//	if (UCYAbilitySystemComponent* ASC = GetAbilitySystemComponent())
//	{
//		//for (const TObjectPtr<const ULyraAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
//		//{
//		//	AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
//		//}
//	}
//	else
//	{
//		//@TODO: Warning logging?
//	}
//
//	//Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
//
//	MarkItemDirty(NewEntry);
//
//	return Result;
//}
//
//UCYItemInstance* FCYEquipmentList::AddEntry(UCYItemInstance* InItemInstance)
//{
//	UCYItemInstance* Result = nullptr;
//
//	check(InItemInstance != nullptr);
//	check(OwnerComponent);
//	check(OwnerComponent->GetOwner()->HasAuthority());
//
//	TSubclassOf<UCYItemDefinition> ItemDef = InItemInstance->GetItemDefinitionClass();
//
//	FCYAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
//	NewEntry.EquipmentDefinition = ItemDef;
//	NewEntry.Instance = InItemInstance;
//	Result = NewEntry.Instance;
//
//	if (UCYAbilitySystemComponent* ASC = GetAbilitySystemComponent())
//	{
//		//for (const TObjectPtr<const ULyraAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
//		//{
//		//	AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
//		//}
//	}
//	else
//	{
//		//@TODO: Warning logging?
//	}
//
//	//Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
//
//	MarkItemDirty(NewEntry);
//
//	return Result;
//}
//
//void FCYEquipmentList::RemoveEntry(UCYItemInstance* Instance)
//{
//	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
//	{
//		FCYAppliedEquipmentEntry& Entry = *EntryIt;
//		if (Entry.Instance == Instance)
//		{
//			if (UCYAbilitySystemComponent* ASC = GetAbilitySystemComponent())
//			{
//				//Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
//			}
//
//			//Instance->DestroyEquipmentActors();
//
//			EntryIt.RemoveCurrent();
//			MarkArrayDirty();
//		}
//	}
//}
//
//UCYAbilitySystemComponent* FCYEquipmentList::GetAbilitySystemComponent() const
//{
//	check(OwnerComponent);
//	AActor* OwningActor = OwnerComponent->GetOwner();
//	return Cast<UCYAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
//}

//////////////////////////////////////////////////////////////////////
// UCYEquipmentComponent




UCYEquipmentComponent::UCYEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UCYEquipmentComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UCYItemInstance* UCYEquipmentComponent::EquipItem(TSubclassOf<UCYItemDefinition> InItemDefinition)
{
	UCYItemInstance* Result = nullptr;
	if (InItemDefinition != nullptr)
	{
		Result = EquipmentList.AddItem(InItemDefinition);
		if (Result != nullptr)
		{
			SpawnItemActor(Result);
			GiveAbilityToActor(Result);
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

UCYItemInstance* UCYEquipmentComponent::EquipItemInstance(UCYItemInstance* InItemInstance)
{
	UCYItemInstance* Result = nullptr;
	if (InItemInstance != nullptr)
	{
		Result = EquipmentList.AddItem(InItemInstance, true);
		if (Result != nullptr)
		{
			SpawnItemActor(Result);
			GiveAbilityToActor(Result);
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}

	return Result;
}

UE_DISABLE_OPTIMIZATION
void UCYEquipmentComponent::SpawnItemActor(UCYItemInstance* InItemInstance)
{
	if (APawn* OwningPawn = Cast<APawn>(GetOwner()))
	{
		if (const UCYItemFragment_EquippableItem* EquipInfo = InItemInstance->FindFragmentByClass<UCYItemFragment_EquippableItem>())
		{
			const TArray<FCYEquipmentActorToSpawn> ActorsToSpawn = EquipInfo->ActorsToSpawn;

			InItemInstance->SpawnEquipmentActors(ActorsToSpawn);
		}
	}
}
UE_ENABLE_OPTIMIZATION

void UCYEquipmentComponent::GiveAbilityToActor(UCYItemInstance* InItemInstance)
{
	if (UCYAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		//for (const TObjectPtr<const ULyraAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		//{
		//	AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		//}
	}
}

void UCYEquipmentComponent::UnequipItem(UCYItemInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveItem(ItemInstance);
	}
}

bool UCYEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FCYItemEntry& Entry : EquipmentList.GetItemList())
	{
		UCYItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UCYEquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCYEquipmentComponent::UninitializeComponent()
{
	TArray<UCYItemInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FCYItemEntry& Entry : EquipmentList.GetItemList())
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UCYItemInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UCYEquipmentComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing LyraEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FCYItemEntry& Entry : EquipmentList.GetItemList())
		{
			UCYItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UCYItemInstance* UCYEquipmentComponent::GetFirstInstanceOfType(TSubclassOf<UCYItemInstance> InstanceType)
{
	for (FCYItemEntry& Entry : EquipmentList.GetItemList())
	{
		if (UCYItemInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UCYItemInstance*> UCYEquipmentComponent::GetEquipmentInstancesOfType(TSubclassOf<UCYItemInstance> InstanceType)
{
	TArray<UCYItemInstance*> Results;
	for (const FCYItemEntry& Entry : EquipmentList.GetItemList())
	{
		if (UCYItemInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

UCYAbilitySystemComponent* UCYEquipmentComponent::GetAbilitySystemComponent()
{
	AActor* OwningActor = GetOwner();
	return Cast<UCYAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

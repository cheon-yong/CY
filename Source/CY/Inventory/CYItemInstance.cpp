// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/CYItemInstance.h"

#include "Inventory/CYItemDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/CYItemFragment_EquippableItem.h"
#include "Actor/CYItemActor.h"

UCYItemInstance::UCYItemInstance()
{
}

void UCYItemInstance::Init(TSubclassOf<UCYItemDefinition> InItemDefinitionClass, int32 InStackCount)
{
	ItemDefinitionClass = InItemDefinitionClass;
	StackCount = InStackCount;
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

TSubclassOf<UCYItemDefinition> UCYItemInstance::GetItemDefinitionClass() const
{
	return ItemDefinitionClass;
}

const UCYItemFragment* UCYItemInstance::FindFragmentByClass(TSubclassOf<UCYItemFragment> FragmentClass) const
{
	if ((ItemDefinitionClass != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UCYItemDefinition>(ItemDefinitionClass)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

APawn* UCYItemInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCYItemInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UCYItemInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UCYItemInstance::OnUnequipped()
{
	K2_OnUnequipped();
	
}

UE_DISABLE_OPTIMIZATION
void UCYItemInstance::SpawnEquipmentActors(const TArray<FCYEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}
		
		for (const FCYEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			ACYItemActor* NewActor = GetWorld()->SpawnActorDeferred<ACYItemActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->Init(this);
			NewActor->SetItemState(EItemState::Equipped);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}
UE_ENABLE_OPTIMIZATION

void UCYItemInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UCYItemInstance::OnRep_Instigator()
{
}

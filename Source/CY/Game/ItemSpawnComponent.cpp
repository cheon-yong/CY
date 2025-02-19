// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ItemSpawnComponent.h"

#include "Inventory/CYItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Actor/CYSection.h"
#include "Game/FieldItemData.h"


UItemSpawnComponent::UItemSpawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bInitialized = false;
}

void UItemSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UItemSpawnComponent::Init()
{

	GetAllSectionInWorld();
	CalculateItemRatio();

	bInitialized = true;
	OnInitialize.Broadcast();
}

void UItemSpawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}
void UItemSpawnComponent::GetAllSectionInWorld()
{
	if (UWorld* World = GetWorld())
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (ACYSection* Section = Cast<ACYSection>(Actor))
			{
				Sections.Add(Section);
			}
		}
	}
}


// Create Random Item 
// TO DO : 알고리즘 활용해보기. priority_queue 등
void UItemSpawnComponent::CalculateItemRatio()
{
	for (FItemToSpawn& ItemData : FieldItemData->GetItemsToSpawn())
	{
		int CountPerSection = ItemData.CountToCreate / Sections.Num();

		for (TObjectPtr<ACYSection> Section : Sections)
		{
			Section->SetOrAddItemDefWithCount(ItemData.ItemDef, CountPerSection);
		}

		int remainCount = ItemData.CountToCreate - (CountPerSection * Sections.Num());
		int index = 0;
		while(remainCount > 0)
		{
			Sections[index]->SetOrAddItemDefWithCount(ItemData.ItemDef, 1);
			index++;
			remainCount--;
		}
	}
}


void UItemSpawnComponent::SpawnItems()
{

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ItemSpawnComponent.h"

#include "Inventory/CYItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Actor/CYSection.h"


UItemSpawnComponent::UItemSpawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UItemSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	GetAllSectionInWorld();
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


void UItemSpawnComponent::SpawnItems()
{

	/*for (TSubclassOf<UCYItemDefinition> ItemDef : ItemsToSpawn)
	{

	}*/
}

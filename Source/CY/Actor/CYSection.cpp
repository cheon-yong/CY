// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYSection.h"
#include "Components/BoxComponent.h"
#include "Inventory/CYItemDefinition.h"
#include "Actor/CYItemSpawnPoint.h"
#include "Actor/CYItemActor.h"
#include "Inventory/CYItemInstance.h"
#include "Inventory/CYItemFragment_ItemInfo.h"
#include "Inventory/CYItemFragment_SpawnableToField.h"
#include "Templates/SharedPointer.h"
#include "Game/CYGameState.h"
#include "Game/ItemSpawnComponent.h"

// Sets default values
ACYSection::ACYSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxArea"));
	RootComponent = Area;
}

// Called when the game starts or when spawned
void ACYSection::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::Init);
}



void ACYSection::Init()
{
	GetAllSpawnPoints();

	if (ACYGameState* GameState = Cast<ACYGameState>(GetWorld()->GetGameState<AGameStateBase>()))
	{
		if (UItemSpawnComponent* SpawnComponent = GameState->GetComponentByClass<UItemSpawnComponent>())
		{
			if (SpawnComponent->bInitialized)
			{
				SpawnItemToPoint();
			}
			else
			{
				SpawnComponent->OnInitialize.AddDynamic(this, &ThisClass::SpawnItemToPoint);
			}
		}
	}
}

void ACYSection::GetAllSpawnPoints()
{
	TArray<AActor*> OverlappingActors;
	Area->GetOverlappingActors(OverlappingActors, ACYItemSpawnPoint::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		if (ACYItemSpawnPoint* SpawnPoint = Cast<ACYItemSpawnPoint>(Actor))
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}
}


// TODO
TSubclassOf<UCYItemDefinition> ACYSection::GetRandomItemDefByTag(FGameplayTag ItemType)
{
	/*if (!ItemTypeMap.Contains(ItemType))
		return nullptr;

	TArray<TSubclassOf<UCYItemDefinition>> Items;
	ItemTypeMap.MultiFind(ItemType, Items);
	if (Items.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		return Items[RandomIndex];
	}*/

	return nullptr;
}

void ACYSection::SpawnItemToPoint()
{
	TArray<TSubclassOf<UCYItemDefinition>> Items;
	ItemDefToCreate.GetKeys(Items);

	for (auto Item : Items)
	{
		for (int i = 0; i < ItemDefToCreate[Item]; i++)
		{
			ACYItemSpawnPoint* SpawnPoint = GetRandomSpawnPoint();
			if (SpawnPoint == nullptr)
				continue;

			UCYItemInstance* ItemInstance = NewObject<UCYItemInstance>();
			ItemInstance->Init(Item, 1);

			if (const UCYItemFragment_SpawnableToField* ItemFragment = ItemInstance->FindFragmentByClass<UCYItemFragment_SpawnableToField>())
			{
				ACYItemActor* NewActor = GetWorld()->SpawnActorDeferred<ACYItemActor>(ItemFragment->ActorToSpawn, FTransform::Identity);
				NewActor->Init(ItemInstance);
				NewActor->SetItemState(EItemState::Dropped);
				NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
				NewActor->SetActorTransform(SpawnPoint->GetTransform());
			}
		}
	}

}

TObjectPtr<ACYItemSpawnPoint> ACYSection::GetRandomSpawnPoint()
{
	if (SpawnPoints.Num() == 0)
	{
		return nullptr; // 빈 배열일 경우 기본값 반환
	}

	int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	return SpawnPoints[RandomIndex];
}

void ACYSection::SetOrAddItemDefWithCount(TSubclassOf<UCYItemDefinition> ItemDef, int32 Count)
{
	if (ItemDefToCreate.Contains(ItemDef))
	{
		ItemDefToCreate[ItemDef] += Count;
	}
	else
	{
		ItemDefToCreate.Add(ItemDef, Count);
	}
	
}

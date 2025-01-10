// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYItemActor.h"

#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Inventory/CYItemInstance.h"

// Sets default values
ACYItemActor::ACYItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	bReplicates = true;
	SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ACYItemActor::Init(UCYItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

// Called when the game starts or when spawned
void ACYItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACYItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemInstance);
	DOREPLIFETIME(ThisClass, ItemState);

}

void ACYItemActor::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::None :
		break;
	case EItemState::Equipped :
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetGenerateOverlapEvents(false);
		break;
	case EItemState::Dropped :
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		break;
	case EItemState::MAX :
		break;

	default:
		break;
	}
}

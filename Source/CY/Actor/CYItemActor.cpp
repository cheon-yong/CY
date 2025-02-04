// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYItemActor.h"

#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Inventory/CYItemInstance.h"
#include "Inventory/CYItemDefinition.h"

// Sets default values
ACYItemActor::ACYItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	bReplicates = true;
	SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void ACYItemActor::Init(UCYItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

void ACYItemActor::OnDropped()
{
	ItemState = EItemState::Dropped;

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector Forward = ActorOwner->GetActorForwardVector();

		const float droppedItemDist = 100.f;
		const float droppedItemTraceDist = 100.f;

		const FVector TraceStart = Location + Forward * droppedItemDist;
		const FVector TraceEnd = TraceStart - FVector::UpVector * droppedItemTraceDist;

		TArray<AActor*> ActorsToIgnore = { GetOwner() };

		FHitResult TraceHit;

		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;

		FVector TargetLocation = TraceEnd;

		EDrawDebugTrace::Type DebugDrawType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"),
			true, ActorsToIgnore, DebugDrawType, TraceHit, true))
		{
			if (TraceHit.bBlockingHit)
			{
				TargetLocation = TraceHit.Location;
			}
		}

		SetActorLocation(TargetLocation);

		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
	}
}

void ACYItemActor::OnUse()
{
}

// Called when the game starts or when spawned
void ACYItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		if (!(IsValid(ItemInstance)) && IsValid(ItemDefinitionClass))
		{
			ItemInstance = NewObject<UCYItemInstance>();
			ItemInstance->Init(ItemDefinitionClass);

			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SphereComponent->SetGenerateOverlapEvents(true);
		}
	}
}

void ACYItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemInstance);
	DOREPLIFETIME(ThisClass, ItemState);
	DOREPLIFETIME(ThisClass, SkeletalMeshComponent);
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

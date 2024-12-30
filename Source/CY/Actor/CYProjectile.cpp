// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYProjectile.h"

#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

// Sets default values
ACYProjectile::ACYProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SMC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ACYProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle LifeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle, 
		this, 
		&ThisClass::DestroyProjectile,
		LifeTime, 
		false
	);
}

void ACYProjectile::DestroyProjectile()
{
	SMC->SetHiddenInGame(true);
	PMC->Deactivate();

	Destroy();
}

// Called every frame
void ACYProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


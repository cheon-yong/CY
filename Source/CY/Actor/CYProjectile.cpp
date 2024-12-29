// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	
}

// Called every frame
void ACYProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


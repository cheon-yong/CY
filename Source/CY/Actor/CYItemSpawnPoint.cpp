// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYItemSpawnPoint.h"

#include "Components/SphereComponent.h"

// Sets default values
ACYItemSpawnPoint::ACYItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACYItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYSection.h"
#include "Components/BoxComponent.h"

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
	

}
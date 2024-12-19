// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYPlayerController.h"
#include "CYPlayerController.h"
#include "CY.h"

void ACYPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACYPlayerController::PostNetInit()
{
	Super::PostNetInit();

}

void ACYPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void ACYPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

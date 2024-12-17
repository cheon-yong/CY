// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYPlayerController.h"
#include "CYPlayerController.h"
#include "CY.h"

void ACYPlayerController::PostInitializeComponents()
{
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	Super::PostInitializeComponents();
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));	
}

void ACYPlayerController::PostNetInit()
{
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	Super::PostNetInit();


	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		CY_LOG(LogCYNetwork, Log, TEXT("Server Connections : %s"), *NetDriver->ServerConnection->GetName());
	}
	else
	{
		CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
}

void ACYPlayerController::BeginPlay()
{
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	Super::BeginPlay();
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void ACYPlayerController::OnPossess(APawn* InPawn)
{
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	Super::OnPossess(InPawn);
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
}

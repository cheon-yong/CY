// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CYGameState.h"
#include "CY.h"

void ACYGameState::HandleBeginPlay()
{
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	Super::HandleBeginPlay();
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
}

void ACYGameState::OnRep_ReplicatedHasBegunPlay()
{
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	Super::OnRep_ReplicatedHasBegunPlay();
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
}

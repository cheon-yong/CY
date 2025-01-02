// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/CYGameMode.h"
#include "Character/CYCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "CY.h"
#include "Game/CYGameState.h"

ACYGameMode::ACYGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = ACYGameState::StaticClass();
}

//void ACYGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("======================================================"));
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
//	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
//	//ErrorMessage = TEXT("Server is full");
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
//}
//
//APlayerController* ACYGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
//	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
//	return NewPlayerController;
//}
//
//void ACYGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
//	Super::PostLogin(NewPlayer);
//
//
//	UNetDriver* NetDriver = GetNetDriver();
//	if (NetDriver)
//	{
//		if (NetDriver->ClientConnections.Num() == 0)
//		{
//			CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("No Client Connection"));
//		}
//		else
//		{
//			for (const auto& Connection : NetDriver->ClientConnections)
//			{
//				CY_LOG(LogCYNetwork, Log, TEXT("Client Connections : %s"), *Connection->GetName());
//			}
//		}
//	}
//	else
//	{
//		CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
//	}
//
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
//}
//
//void ACYGameMode::StartPlay()
//{
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
//	Super::StartPlay();
//	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
//}

void ACYGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}

UClass* ACYGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ACYGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

bool ACYGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void ACYGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

}

AActor* ACYGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACYGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ACYGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return Super::PlayerCanRestart(Player);
}

void ACYGameMode::InitGameState()
{
	Super::InitGameState();

}

bool ACYGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return true;
}

void ACYGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

void ACYGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);
}

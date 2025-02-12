// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CYGameState.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Async/TaskGraphInterfaces.h"
#include "Net/UnrealNetwork.h"
#include "Messages/CYVerbMessage.h"
#include "GameFramework/PlayerState.h"
#include "CY.h"


extern ENGINE_API float GAverageFPS;

ACYGameState::ACYGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCYAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ServerFPS = 0.0f;
}

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

void ACYGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACYGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

void ACYGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACYGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		ServerFPS = GAverageFPS;
	}
}

void ACYGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ACYGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

void ACYGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	// Remove inactive and bots
	for (int32 i = PlayerArray.Num() - 1; i >= 0; i--)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}

UAbilitySystemComponent* ACYGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent; return nullptr;
}


void ACYGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerFPS);
	DOREPLIFETIME_CONDITION(ThisClass, RecorderPlayerState, COND_ReplayOnly);
}

void ACYGameState::MulticastMessageToClients_Implementation(const FCYVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void ACYGameState::MulticastReliableMessageToClients_Implementation(const FCYVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}

float ACYGameState::GetServerFPS() const
{
	return ServerFPS;
}

void ACYGameState::SetRecorderPlayerState(APlayerState* NewPlayerState)
{
	if (RecorderPlayerState == nullptr)
	{
		// Set it and call the rep callback so it can do any record-time setup
		RecorderPlayerState = NewPlayerState;
		OnRep_RecorderPlayerState();
	}
	else
	{
		//UE_LOG(LogLyra, Warning, TEXT("SetRecorderPlayerState was called on %s but should only be called once per game on the primary user"), *GetName());
	}
}

APlayerState* ACYGameState::GetRecorderPlayerState() const
{
	return RecorderPlayerState;
}

void ACYGameState::OnRep_RecorderPlayerState()
{
	OnRecorderPlayerStateChangedEvent.Broadcast(RecorderPlayerState);
}

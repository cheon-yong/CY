// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"

ACYPlayerState::ACYPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ASC = ObjectInitializer.CreateDefaultSubobject<UCYAbilitySystemComponent>(this, TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	
	NetUpdateFrequency = 100.0f;

	AttributeSet = CreateDefaultSubobject<UCYAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACYPlayerState::GetAbilitySystemComponent() const
{
	return GetCYAbilitySystemComponent();
}

void ACYPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void ACYPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
}

void ACYPlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
	else
	{
		//UE_LOG(LogLyraTeams, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}

FGenericTeamId ACYPlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnCYTeamIndexChangedDelegate* ACYPlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ACYPlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void ACYPlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 ACYPlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool ACYPlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void ACYPlayerState::ClientBroadcastMessage_Implementation(const FCYVerbMessage Message)
{
	// This check is needed to prevent running the action when in standalone mode
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

FRotator ACYPlayerState::GetReplicatedViewRotation() const
{
	return FRotator();
}

void ACYPlayerState::SetReplicatedViewRotation(const FRotator& NewRotation)
{
}

void ACYPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

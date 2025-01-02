// Copyright Epic Games, Inc. All Rights Reserved.

#include "CYTeamInfoBase.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Team/CYTeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CYTeamInfoBase)

class FLifetimeProperty;

ACYTeamInfoBase::ACYTeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TeamId(INDEX_NONE)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void ACYTeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

void ACYTeamInfoBase::BeginPlay()
{
	Super::BeginPlay();

	TryRegisterWithTeamSubsystem();
}

void ACYTeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TeamId != INDEX_NONE)
	{
		UCYTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UCYTeamSubsystem>();
		if (TeamSubsystem)
		{
			// EndPlay can happen at weird times where the subsystem has already been destroyed
			TeamSubsystem->UnregisterTeamInfo(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void ACYTeamInfoBase::RegisterWithTeamSubsystem(UCYTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void ACYTeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		UCYTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UCYTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			RegisterWithTeamSubsystem(TeamSubsystem);
		}
	}
}

void ACYTeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void ACYTeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}


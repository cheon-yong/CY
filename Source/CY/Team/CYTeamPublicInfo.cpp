// Copyright Epic Games, Inc. All Rights Reserved.

#include "CYTeamPublicInfo.h"

#include "Net/UnrealNetwork.h"
#include "Team/CYTeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CYTeamPublicInfo)

class FLifetimeProperty;

ACYTeamPublicInfo::ACYTeamPublicInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ACYTeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void ACYTeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UCYTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void ACYTeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}


// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team/CYTeamCreationComponent.h"
#include "CYTeamPublicInfo.h"
#include "CYTeamPrivateInfo.h"
#include "Player/CYPlayerState.h"
#include "Engine/World.h"
#include "Game/CYGameMode.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(CYTeamCreationComponent)

UCYTeamCreationComponent::UCYTeamCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PublicTeamInfoClass = ACYTeamPublicInfo::StaticClass();
	PrivateTeamInfoClass = ACYTeamPrivateInfo::StaticClass();
}

#if WITH_EDITOR
EDataValidationResult UCYTeamCreationComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	//@TODO: TEAMS: Validate that all display assets have the same properties set!

	return Result;
}
#endif

void UCYTeamCreationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	/*AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_HighPriority(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));*/
}

//void UCYTeamCreationComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
//{
//#if WITH_SERVER_CODE
//	if (HasAuthority())
//	{
//		ServerCreateTeams();
//		ServerAssignPlayersToTeams();
//	}
//#endif
//}

#if WITH_SERVER_CODE

void UCYTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& KVP : TeamsToCreate)
	{
		const int32 TeamId = KVP.Key;
		ServerCreateTeam(TeamId, KVP.Value);
	}
}

void UCYTeamCreationComponent::ServerAssignPlayersToTeams()
{
	// Assign players that already exist to teams
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ACYPlayerState* CYPS = Cast<ACYPlayerState>(PS))
		{
			ServerChooseTeamForPlayer(CYPS);
		}
	}

	// Listen for new players logging in
	ACYGameMode* GameMode = Cast<ACYGameMode>(GameState->AuthorityGameMode);
	check(GameMode);

	GameMode->OnGameModePlayerInitialized.AddUObject(this, &ThisClass::OnPlayerInitialized);
}

void UCYTeamCreationComponent::ServerChooseTeamForPlayer(ACYPlayerState* PS)
{
	if (PS->IsOnlyASpectator())
	{
		PS->SetGenericTeamId(FGenericTeamId::NoTeam);
	}
	else
	{
		const FGenericTeamId TeamID = IntegerToGenericTeamId(GetLeastPopulatedTeamID());
		PS->SetGenericTeamId(TeamID);
	}
}

void UCYTeamCreationComponent::OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState);
	if (ACYPlayerState* CYPS = Cast<ACYPlayerState>(NewPlayer->PlayerState))
	{
		ServerChooseTeamForPlayer(CYPS);
	}
}

void UCYTeamCreationComponent::ServerCreateTeam(int32 TeamId, UCYTeamDisplayAsset* DisplayAsset)
{
	check(HasAuthority());

	//@TODO: ensure the team doesn't already exist

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACYTeamPublicInfo* NewTeamPublicInfo = World->SpawnActor<ACYTeamPublicInfo>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"), *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamId(TeamId);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset);

	ACYTeamPrivateInfo* NewTeamPrivateInfo = World->SpawnActor<ACYTeamPrivateInfo>(PrivateTeamInfoClass, SpawnInfo);
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"), *GetPathNameSafe(*PrivateTeamInfoClass));
	NewTeamPrivateInfo->SetTeamId(TeamId);
}

int32 UCYTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num();
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts;
		TeamMemberCounts.Reserve(NumTeams);

		for (const auto& KVP : TeamsToCreate)
		{
			const int32 TeamId = KVP.Key;
			TeamMemberCounts.Add(TeamId, 0);
		}

		AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (ACYPlayerState* CYPS = Cast<ACYPlayerState>(PS))
			{
				const int32 PlayerTeamID = CYPS->GetTeamId();

				if ((PlayerTeamID != INDEX_NONE) && !CYPS->IsInactive())	// do not count unassigned or disconnected players
				{
					check(TeamMemberCounts.Contains(PlayerTeamID))
					TeamMemberCounts[PlayerTeamID] += 1;
				}
			}
		}

		// sort by lowest team population, then by team ID
		int32 BestTeamId = INDEX_NONE;
		uint32 BestPlayerCount = TNumericLimits<uint32>::Max();
		for (const auto& KVP : TeamMemberCounts)
		{
			const int32 TestTeamId = KVP.Key;
			const uint32 TestTeamPlayerCount = KVP.Value;

			if (TestTeamPlayerCount < BestPlayerCount)
			{
				BestTeamId = TestTeamId;
				BestPlayerCount = TestTeamPlayerCount;
			}
			else if (TestTeamPlayerCount == BestPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId;
					BestPlayerCount = TestTeamPlayerCount;
				}
			}
		}

		return BestTeamId;
	}

	return INDEX_NONE;
}
#endif	// WITH_SERVER_CODE
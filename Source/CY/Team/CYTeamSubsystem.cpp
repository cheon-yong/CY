// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team/CYTeamSubsystem.h"

#include "AbilitySystemGlobals.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "CYTeamAgentInterface.h"
#include "CYTeamPrivateInfo.h"
#include "CYTeamPublicInfo.h"
#include "Player/CYPlayerState.h"
#include "Team/CYTeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CYTeamSubsystem)

class FSubsystemCollectionBase;

//////////////////////////////////////////////////////////////////////
// FCYTeamTrackingInfo

void FCYTeamTrackingInfo::SetTeamInfo(ACYTeamInfoBase* Info)
{
	if (ACYTeamPublicInfo* NewPublicInfo = Cast<ACYTeamPublicInfo>(Info))
	{
		ensure((PublicInfo == nullptr) || (PublicInfo == NewPublicInfo));
		PublicInfo = NewPublicInfo;

		UCYTeamDisplayAsset* OldDisplayAsset = DisplayAsset;
		DisplayAsset = NewPublicInfo->GetTeamDisplayAsset();

		if (OldDisplayAsset != DisplayAsset)
		{
			OnTeamDisplayAssetChanged.Broadcast(DisplayAsset);
		}
	}
	else if (ACYTeamPrivateInfo* NewPrivateInfo = Cast<ACYTeamPrivateInfo>(Info))
	{
		ensure((PrivateInfo == nullptr) || (PrivateInfo == NewPrivateInfo));
		PrivateInfo = NewPrivateInfo;
	}
	else
	{
		checkf(false, TEXT("Expected a public or private team info but got %s"), *GetPathNameSafe(Info))
	}
}

void FCYTeamTrackingInfo::RemoveTeamInfo(ACYTeamInfoBase* Info)
{
	if (PublicInfo == Info)
	{
		PublicInfo = nullptr;
	}
	else if (PrivateInfo == Info)
	{
		PrivateInfo = nullptr;
	}
	else
	{
		ensureMsgf(false, TEXT("Expected a previously registered team info but got %s"), *GetPathNameSafe(Info));
	}
}

//////////////////////////////////////////////////////////////////////
// UCYTeamSubsystem

UCYTeamSubsystem::UCYTeamSubsystem()
{
}

void UCYTeamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	/*auto AddTeamCheats = [](UCheatManager* CheatManager)
	{
		CheatManager->AddCheatManagerExtension(NewObject<UCYTeamCheats>(CheatManager));
	};

	CheatManagerRegistrationHandle = UCheatManager::RegisterForOnCheatManagerCreated(FOnCheatManagerCreated::FDelegate::CreateLambda(AddTeamCheats));*/
}

void UCYTeamSubsystem::Deinitialize()
{
	//UCheatManager::UnregisterFromOnCheatManagerCreated(CheatManagerRegistrationHandle);

	Super::Deinitialize();
}

bool UCYTeamSubsystem::RegisterTeamInfo(ACYTeamInfoBase* TeamInfo)
{
	if (!ensure(TeamInfo))
	{
		return false;
	}

	const int32 TeamId = TeamInfo->GetTeamId();
	if (ensure(TeamId != INDEX_NONE))
	{
		FCYTeamTrackingInfo& Entry = TeamMap.FindOrAdd(TeamId);
		Entry.SetTeamInfo(TeamInfo);

		return true;
	}

	return false;
}

bool UCYTeamSubsystem::UnregisterTeamInfo(ACYTeamInfoBase* TeamInfo)
{
	if (!ensure(TeamInfo))
	{
		return false;
	}

	const int32 TeamId = TeamInfo->GetTeamId();
	if (ensure(TeamId != INDEX_NONE))
	{
		FCYTeamTrackingInfo* Entry = TeamMap.Find(TeamId);

		// If it couldn't find the entry, this is probably a leftover actor from a previous world, ignore it
		if (Entry)
		{
			Entry->RemoveTeamInfo(TeamInfo);

			return true;
		}
	}

	return false;
}

bool UCYTeamSubsystem::ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex)
{
	const FGenericTeamId NewTeamID = IntegerToGenericTeamId(NewTeamIndex);
	if (ACYPlayerState* CYPS = const_cast<ACYPlayerState*>(FindPlayerStateFromActor(ActorToChange)))
	{
		CYPS->SetGenericTeamId(NewTeamID);
		return true;
	}
	else if (ICYTeamAgentInterface* TeamActor = Cast<ICYTeamAgentInterface>(ActorToChange))
	{
		TeamActor->SetGenericTeamId(NewTeamID);
		return true;
	}
	else
	{
		return false;
	}
}

int32 UCYTeamSubsystem::FindTeamFromObject(const UObject* TestObject) const
{
	// See if it's directly a team agent
	if (const ICYTeamAgentInterface* ObjectWithTeamInterface = Cast<ICYTeamAgentInterface>(TestObject))
	{
		return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
	}

	if (const AActor* TestActor = Cast<const AActor>(TestObject))
	{
		// See if the instigator is a team actor
		if (const ICYTeamAgentInterface* InstigatorWithTeamInterface = Cast<ICYTeamAgentInterface>(TestActor->GetInstigator()))
		{
			return GenericTeamIdToInteger(InstigatorWithTeamInterface->GetGenericTeamId());
		}

		// TeamInfo actors don't actually have the team interface, so they need a special case
		if (const ACYTeamInfoBase* TeamInfo = Cast<ACYTeamInfoBase>(TestActor))
		{
			return TeamInfo->GetTeamId();
		}

		// Fall back to finding the associated player state
		if (const ACYPlayerState* CYPS = FindPlayerStateFromActor(TestActor))
		{
			return CYPS->GetTeamId();
		}
	}

	return INDEX_NONE;
}

const ACYPlayerState* UCYTeamSubsystem::FindPlayerStateFromActor(const AActor* PossibleTeamActor) const
{
	if (PossibleTeamActor != nullptr)
	{
		if (const APawn* Pawn = Cast<const APawn>(PossibleTeamActor))
		{
			//@TODO: Consider an interface instead or have team actors register with the subsystem and have it maintain a map? (or LWC style)
			if (ACYPlayerState* CYPS = Pawn->GetPlayerState<ACYPlayerState>())
			{
				return CYPS;
			}
		}
		else if (const AController* PC = Cast<const AController>(PossibleTeamActor))
		{
			if (ACYPlayerState* CYPS = Cast<ACYPlayerState>(PC->PlayerState))
			{
				return CYPS;
			}
		}
		else if (const ACYPlayerState* CYPS = Cast<const ACYPlayerState>(PossibleTeamActor))
		{
			return CYPS; 
		}

		// Try the instigator
// 		if (AActor* Instigator = PossibleTeamActor->GetInstigator())
// 		{
// 			if (ensure(Instigator != PossibleTeamActor))
// 			{
// 				return FindPlayerStateFromActor(Instigator);
// 			}
// 		}
	}

	return nullptr;
}

ECYTeamComparison UCYTeamSubsystem::CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const
{
	TeamIdA = FindTeamFromObject(Cast<const AActor>(A));
	TeamIdB = FindTeamFromObject(Cast<const AActor>(B));

	if ((TeamIdA == INDEX_NONE) || (TeamIdB == INDEX_NONE))
	{
		return ECYTeamComparison::InvalidArgument;
	}
	else
	{
		return (TeamIdA == TeamIdB) ? ECYTeamComparison::OnSameTeam : ECYTeamComparison::DifferentTeams;
	}
}

ECYTeamComparison UCYTeamSubsystem::CompareTeams(const UObject* A, const UObject* B) const
{
	int32 TeamIdA;
	int32 TeamIdB;
	return CompareTeams(A, B, /*out*/ TeamIdA, /*out*/ TeamIdB);
}

void UCYTeamSubsystem::FindTeamFromActor(const UObject* TestObject, bool& bIsPartOfTeam, int32& TeamId) const
{
	TeamId = FindTeamFromObject(TestObject);
	bIsPartOfTeam = TeamId != INDEX_NONE;
}

void UCYTeamSubsystem::AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount)
{
	auto FailureHandler = [&](const FString& ErrorMessage)
	{
		//UE_LOG(LogCYTeams, Error, TEXT("AddTeamTagStack(TeamId: %d, Tag: %s, StackCount: %d) %s"), TeamId, *Tag.ToString(), StackCount, *ErrorMessage);
	};

	if (FCYTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		if (Entry->PublicInfo)
		{
			if (Entry->PublicInfo->HasAuthority())
			{
				Entry->PublicInfo->TeamTags.AddStack(Tag, StackCount);
			}
			else
			{
				FailureHandler(TEXT("failed because it was called on a client"));
			}
		}
		else
		{
			FailureHandler(TEXT("failed because there is no team info spawned yet (called too early, before the experience was ready)"));
		}
	}
	else
	{
		FailureHandler(TEXT("failed because it was passed an unknown team id"));
	}
}

void UCYTeamSubsystem::RemoveTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount)
{
	auto FailureHandler = [&](const FString& ErrorMessage)
	{
		//UE_LOG(LogCYTeams, Error, TEXT("RemoveTeamTagStack(TeamId: %d, Tag: %s, StackCount: %d) %s"), TeamId, *Tag.ToString(), StackCount, *ErrorMessage);
	};

	if (FCYTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		if (Entry->PublicInfo)
		{
			if (Entry->PublicInfo->HasAuthority())
			{
				Entry->PublicInfo->TeamTags.RemoveStack(Tag, StackCount);
			}
			else
			{
				FailureHandler(TEXT("failed because it was called on a client"));
			}
		}
		else
		{
			FailureHandler(TEXT("failed because there is no team info spawned yet (called too early, before the experience was ready)"));
		}
	}
	else
	{
		FailureHandler(TEXT("failed because it was passed an unknown team id"));
	}
}

int32 UCYTeamSubsystem::GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const
{
	if (const FCYTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		const int32 PublicStackCount = (Entry->PublicInfo != nullptr) ? Entry->PublicInfo->TeamTags.GetStackCount(Tag) : 0;
		const int32 PrivateStackCount = (Entry->PrivateInfo != nullptr) ? Entry->PrivateInfo->TeamTags.GetStackCount(Tag) : 0;
		return PublicStackCount + PrivateStackCount;
	}
	else
	{
		//UE_LOG(LogCYTeams, Verbose, TEXT("GetTeamTagStackCount(TeamId: %d, Tag: %s) failed because it was passed an unknown team id"), TeamId, *Tag.ToString());
		return 0;
	}
}

bool UCYTeamSubsystem::TeamHasTag(int32 TeamId, FGameplayTag Tag) const
{
	return GetTeamTagStackCount(TeamId, Tag) > 0;
}

bool UCYTeamSubsystem::DoesTeamExist(int32 TeamId) const
{
	return TeamMap.Contains(TeamId);
}

TArray<int32> UCYTeamSubsystem::GetTeamIDs() const
{
	TArray<int32> Result;
	TeamMap.GenerateKeyArray(Result);
	Result.Sort();
	return Result;
}

bool UCYTeamSubsystem::CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf) const
{
	if (bAllowDamageToSelf)
	{
		if ((Instigator == Target) || (FindPlayerStateFromActor(Cast<AActor>(Instigator)) == FindPlayerStateFromActor(Cast<AActor>(Target))))
		{
			return true;
		}
	}

	int32 InstigatorTeamId;
	int32 TargetTeamId;
	const ECYTeamComparison Relationship = CompareTeams(Instigator, Target, /*out*/ InstigatorTeamId, /*out*/ TargetTeamId);
	if (Relationship == ECYTeamComparison::DifferentTeams)
	{
		return true;
	}
	else if ((Relationship == ECYTeamComparison::InvalidArgument) && (InstigatorTeamId != INDEX_NONE))
	{
		// Allow damaging non-team actors for now, as long as they have an ability system component
		//@TODO: This is temporary until the target practice dummy has a team assignment
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Cast<const AActor>(Target)) != nullptr;
	}

	return false;
}

UCYTeamDisplayAsset* UCYTeamSubsystem::GetTeamDisplayAsset(int32 TeamId, int32 ViewerTeamId)
{
	// Currently ignoring ViewerTeamId

	if (FCYTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		return Entry->DisplayAsset;
	}

	return nullptr;
}

UCYTeamDisplayAsset* UCYTeamSubsystem::GetEffectiveTeamDisplayAsset(int32 TeamId, UObject* ViewerTeamAgent)
{
	return GetTeamDisplayAsset(TeamId, FindTeamFromObject(ViewerTeamAgent));
}

void UCYTeamSubsystem::NotifyTeamDisplayAssetModified(UCYTeamDisplayAsset* /*ModifiedAsset*/)
{
	// Broadcasting to all observers when a display asset is edited right now, instead of only the edited one
	for (const auto& KVP : TeamMap)
	{
		const int32 TeamId = KVP.Key;
		const FCYTeamTrackingInfo& TrackingInfo = KVP.Value;

		TrackingInfo.OnTeamDisplayAssetChanged.Broadcast(TrackingInfo.DisplayAsset);
	}
}

FOnCYTeamDisplayAssetChangedDelegate& UCYTeamSubsystem::GetTeamDisplayAssetChangedDelegate(int32 TeamId)
{
	return TeamMap.FindOrAdd(TeamId).OnTeamDisplayAssetChanged;
}


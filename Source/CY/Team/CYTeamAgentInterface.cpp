// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team/CYTeamAgentInterface.h"

#include "UObject/ScriptInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CYTeamAgentInterface)

UCYTeamAgentInterface::UCYTeamAgentInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ICYTeamAgentInterface::ConditionalBroadcastTeamChanged(TScriptInterface<ICYTeamAgentInterface> This, FGenericTeamId OldTeamID, FGenericTeamId NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID); 
		const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID);

		UObject* ThisObj = This.GetObject();
		//UE_LOG(LogCYTeams, Verbose, TEXT("[%s] %s assigned team %d"), *GetClientServerContextString(ThisObj), *GetPathNameSafe(ThisObj), NewTeamIndex);

		This.GetInterface()->GetTeamChangedDelegateChecked().Broadcast(ThisObj, OldTeamIndex, NewTeamIndex);
	}
}


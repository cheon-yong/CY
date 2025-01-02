// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CYTeamInfoBase.h"

#include "CYTeamPrivateInfo.generated.h"

class UObject;

UCLASS()
class ACYTeamPrivateInfo : public ACYTeamInfoBase
{
	GENERATED_BODY()

public:
	ACYTeamPrivateInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

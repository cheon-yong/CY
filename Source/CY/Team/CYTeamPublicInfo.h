// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CYTeamInfoBase.h"

#include "CYTeamPublicInfo.generated.h"

class UCYTeamCreationComponent;
class UCYTeamDisplayAsset;
class UObject;
struct FFrame;

UCLASS()
class ACYTeamPublicInfo : public ACYTeamInfoBase
{
	GENERATED_BODY()

	friend UCYTeamCreationComponent;

public:
	ACYTeamPublicInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UCYTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UCYTeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UCYTeamDisplayAsset> TeamDisplayAsset;
};

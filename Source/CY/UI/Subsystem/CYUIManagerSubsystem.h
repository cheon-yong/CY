// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Ticker.h"
#include "GameUIManagerSubsystem.h"


#include "CYUIManagerSubsystem.generated.h"

class FSubsystemCollectionBase;
class UObject;

/**
 * 
 */
UCLASS()
class UCYUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
public:

	UCYUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

	FTSTicker::FDelegateHandle TickHandle;
	
};

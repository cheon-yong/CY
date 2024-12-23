// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CYCharacter.h"
#include "CYNonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CY_API ACYNonPlayerCharacter : public ACYCharacter
{
	GENERATED_BODY()
	
public:
	ACYNonPlayerCharacter();


protected:
	virtual void PossessedBy(AController* NewController) override;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/CYCharacter.h"
#include "Engine/LocalPlayer.h"

#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "UI/CYWidgetComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACYCharacter

ACYCharacter::ACYCharacter()
{
	HpBar = CreateDefaultSubobject<UCYWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
}

UAbilitySystemComponent* ACYCharacter::GetAbilitySystemComponent() const
{
	return GetCYAbilitySystemComponent();
}


void ACYCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ACYCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}


void ACYCharacter::AddCharacterAbilities()
{
	if (ASC)
	{
		ASC->AddAbilities(StartAbilities);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CYNonPlayerCharacter.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"

ACYNonPlayerCharacter::ACYNonPlayerCharacter()
{
	ASC = CreateDefaultSubobject<UCYAbilitySystemComponent>(TEXT("ASC"));

	AttributeSet = CreateDefaultSubobject<UCYAttributeSet>(TEXT("AttributeSet"));
}

void ACYNonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!ASC)
		return;

	ASC->InitAbilityActorInfo(this, this);
}

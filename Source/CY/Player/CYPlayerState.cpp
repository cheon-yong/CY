// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"

ACYPlayerState::ACYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	AttributeSet = CreateDefaultSubobject<UCYAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

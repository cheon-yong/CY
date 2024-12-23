// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CYAttributeSet.h"

ACYPlayerState::ACYPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ASC = ObjectInitializer.CreateDefaultSubobject<UCYAbilitySystemComponent>(this, TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	
	NetUpdateFrequency = 100.0f;

	AttributeSet = CreateDefaultSubobject<UCYAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACYPlayerState::GetAbilitySystemComponent() const
{
	return GetCYAbilitySystemComponent();
}

void ACYPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

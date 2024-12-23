// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/CYGameplayAbility.h"
#include "CYGameplayTags.h"

using namespace CYGameplayTags;

UCYAbilitySystemComponent::UCYAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	SetIsReplicated(true);
}

void UCYAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UCYAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UCYAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
		}
	}
}

void UCYAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
		InputHeldSpecHandles.Remove(AbilitySpec.Handle);
	}
}

void UCYAbilitySystemComponent::AddAbilities(TArray<TSubclassOf<UCYGameplayAbility>> Abilities)
{
	Server_AddAbilities(Abilities);
}

void UCYAbilitySystemComponent::AddInputAbilities(TArray<TSubclassOf<UCYGameplayAbility>> Abilities)
{
	Server_AddInputAbilities(Abilities);
}

bool UCYAbilitySystemComponent::Server_AddAbilities_Validate(const TArray<TSubclassOf<UCYGameplayAbility>>& Abilities)
{
	return true;
}


void UCYAbilitySystemComponent::Server_AddAbilities_Implementation(const TArray<TSubclassOf<UCYGameplayAbility>>& Abilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UCYGameplayAbility* CYAbility = Cast<UCYGameplayAbility>(AbilitySpec.Ability))
		{
			GiveAbility(AbilitySpec);
		}
	}
}

bool UCYAbilitySystemComponent::Server_AddInputAbilities_Validate(const TArray<TSubclassOf<UCYGameplayAbility>>& Abilities)
{
	return true;
}


void UCYAbilitySystemComponent::Server_AddInputAbilities_Implementation(const TArray<TSubclassOf<UCYGameplayAbility>>& Abilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UCYGameplayAbility* CYAbility = Cast<UCYGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(CYAbility->InputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

PRAGMA_DISABLE_OPTIMIZATION
void UCYAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UCYGameplayAbility* CYAbilityCDO = Cast<UCYGameplayAbility>(AbilitySpec->Ability);
				if (CYAbilityCDO && CYAbilityCDO->GetActivationPolicy() == ECYAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UCYGameplayAbility* CYAbilityCDO = Cast<UCYGameplayAbility>(AbilitySpec->Ability);

					if (CYAbilityCDO && CYAbilityCDO->GetActivationPolicy() == ECYAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
PRAGMA_ENABLE_OPTIMIZATION

void UCYAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UCYAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
}

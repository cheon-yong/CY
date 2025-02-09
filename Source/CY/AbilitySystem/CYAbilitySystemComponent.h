// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CYAbilitySystemComponent.generated.h"

class UCYGameplayAbility;

/**
 * 
 */
UCLASS()
class CY_API UCYAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UCYAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void AddAbilities(TArray<TSubclassOf<UCYGameplayAbility>> Abilities);
	void AddInputAbilities(TArray<TSubclassOf<UCYGameplayAbility>> Abilities);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddInputAbilities(const TArray<TSubclassOf<UCYGameplayAbility>>& Abilities);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddAbilities(const TArray<TSubclassOf<UCYGameplayAbility>>& Abilities);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:
	virtual void OnRep_ActivateAbilities() override;

	void TryActivateAbilitiesOnSpawn();

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};

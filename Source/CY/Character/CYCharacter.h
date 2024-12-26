// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "CYCharacter.generated.h"

class AController;
class UCYAbilitySystemComponent;
class UCYGameplayAbility;
class UCYAttributeSet;
class UCYWidgetComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACYCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACYCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const { return ASC; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	virtual void AddCharacterAbilities();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = GAS)
	TObjectPtr<UCYAbilitySystemComponent> ASC;

	UPROPERTY(BlueprintReadOnly, Category = GAS)
	TObjectPtr<UCYAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<UCYGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = UI)
	TObjectPtr<UCYWidgetComponent> HpBar;
};


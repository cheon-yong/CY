// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "Team/CYTeamAgentInterface.h"
#include <AbilitySystem/CYAbilitySet.h>

#include "CYCharacter.generated.h"

class AController;
class UCYAbilitySystemComponent;
class UCYGameplayAbility;
class UCYAttributeSet;
class UCYWidgetComponent;
class UCYInventoryComponent;
struct FInputActionValue;
class UCYEquipmentComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitASCDelegate, UCYAbilitySystemComponent*, ASC);

UCLASS(config=Game)
class ACYCharacter : public ACharacter, public IAbilitySystemInterface, public ICYTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACYCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const { return ASC; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void AddCharacterAbilities();

	//~ICYTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	UFUNCTION(BlueprintCallable, Category = "Team")
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnCYTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ICYTeamAgentInterface interface

	//~APawn interface
	virtual void NotifyControllerChanged() override;
	//~End of APawn interface

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

public:
	UPROPERTY(BlueprintAssignable)
	FOnInitASCDelegate OnInitASC;

protected:
	UPROPERTY(BlueprintReadOnly, Category = GAS)
	TObjectPtr<UCYAbilitySystemComponent> ASC;

	UPROPERTY(BlueprintReadOnly, Category = GAS)
	TObjectPtr<UCYAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TObjectPtr<UCYAbilitySet>> StartAbilitySets;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<UCYGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = UI)
	TObjectPtr<UCYWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, Replicated)
	TObjectPtr<UCYInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Replicated, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCYEquipmentComponent> EquipmentComponent;

private:
	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY()
	FOnCYTeamIndexChangedDelegate OnTeamChangedDelegate;
};


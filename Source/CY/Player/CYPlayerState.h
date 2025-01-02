// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Team/CYTeamAgentInterface.h"

#include "CYPlayerState.generated.h"


class UCYAbilitySystemComponent;
class UCYAttributeSet;
/**
 * 
 */
UCLASS()
class CY_API ACYPlayerState : public APlayerState, public IAbilitySystemInterface, public ICYTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ACYPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const { return ASC; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~ICYTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnCYTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ICYTeamAgentInterface interface

	/** Returns the Team ID of the team the player belongs to. */
	UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}
	
protected:

private:
	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UCYAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UCYAttributeSet> AttributeSet;

private:
	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY()
	FOnCYTeamIndexChangedDelegate OnTeamChangedDelegate;
};

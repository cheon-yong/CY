// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Team/CYTeamAgentInterface.h"
#include "System/GameplayTagStack.h"
#include "Messages/CYVerbMessage.h"

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
	

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category = Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = Teams)
	bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "CY|PlayerState")
	void ClientBroadcastMessage(const FCYVerbMessage Message);

	// Gets the replicated view rotation of this player, used for spectating
	FRotator GetReplicatedViewRotation() const;

	// Sets the replicated view rotation, only valid on the server
	void SetReplicatedViewRotation(const FRotator& NewRotation);


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

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY()
	FOnCYTeamIndexChangedDelegate OnTeamChangedDelegate;
};

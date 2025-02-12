// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/GameStateBase.h"
#include "AbilitySystemInterface.h"

#include "CYGameState.generated.h"


struct FCYVerbMessage;

class APlayerState;
class UAbilitySystemComponent;
class UCYAbilitySystemComponent;
class UObject;
struct FFrame;


/**
 * 
 */
UCLASS()
class CY_API ACYGameState : public AGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACYGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void HandleBeginPlay() override;

	virtual void OnRep_ReplicatedHasBegunPlay() override;

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	//~End of AActor interface

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//~End of AGameStateBase interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	// Gets the ability system component used for game wide things
	UFUNCTION(BlueprintCallable, Category = "CY|GameState")
	UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const { return AbilitySystemComponent; }

	// Send a message that all clients will (probably) get
	// (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "CY|GameState")
	void MulticastMessageToClients(const FCYVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "CY|GameState")
	void MulticastReliableMessageToClients(const FCYVerbMessage Message);

	// Gets the server's FPS, replicated to clients
	float GetServerFPS() const;

	// Indicate the local player state is recording a replay
	void SetRecorderPlayerState(APlayerState* NewPlayerState);

	// Gets the player state that recorded the replay, if valid
	APlayerState* GetRecorderPlayerState() const;

	// Delegate called when the replay player state changes
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecorderPlayerStateChanged, APlayerState*);
	FOnRecorderPlayerStateChanged OnRecorderPlayerStateChangedEvent;

protected:
	UPROPERTY(Replicated)
	float ServerFPS;

	// The player state that recorded a replay, it is used to select the right pawn to follow
	// This is only set in replay streams and is not replicated normally
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RecorderPlayerState)
	TObjectPtr<APlayerState> RecorderPlayerState;

	UFUNCTION()
	void OnRep_RecorderPlayerState();


private:
	UPROPERTY(VisibleAnywhere, Category = "CY|GameState")
	TObjectPtr<UCYAbilitySystemComponent> AbilitySystemComponent;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Team/CYTeamAgentInterface.h"

#include "CYPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UCYInputConfig;
class UCYAbilitySystemComponent;

struct FInputActionValue;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class CY_API ACYPlayerController : public APlayerController, public ICYTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ACYPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	UFUNCTION(BlueprintCallable, Category = "CY|PlayerController")
	UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const;

	// AController Interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface


	// About Ability Input
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	//~ICYTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnCYTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ICYTeamAgentInterface interface


protected:
	// About Default Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();

	virtual void OnPlayerStateChanged();

private:
	UPROPERTY()
	FOnCYTeamIndexChangedDelegate OnTeamChangedDelegate;

	void BroadcastOnPlayerStateChanged();

protected:
	UPROPERTY(BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCYAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCYInputConfig> InputConfig;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};

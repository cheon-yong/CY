// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYPlayerController.h"
#include "CYPlayerController.h"
#include "Character/CYCharacter.h"
#include "Input/CYEnhancedInputComponent.h"
#include "Input/CYInputConfig.h"
#include "CY.h"
#include "GameFramework/Character.h"
#include "Player/CYPlayerState.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include <EnhancedInputSubsystems.h>
#include <Engine.h>

ACYPlayerController::ACYPlayerController()
{
	bReplicates = true;
}

void ACYPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACYPlayerController::PostNetInit()
{
	Super::PostNetInit();

}

void ACYPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void ACYPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}

	if (UCYEnhancedInputComponent* CYInputComponent = Cast<UCYEnhancedInputComponent>(InputComponent))
	{
		CYInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACYPlayerController::Move);
		CYInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACYPlayerController::Look);
		CYInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACYPlayerController::Jump);
		CYInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACYPlayerController::StopJumping);

		CYInputComponent->BindAbilityActions(InputConfig, this, &ACYPlayerController::AbilityInputTagPressed, &ACYPlayerController::AbilityInputTagReleased, &ACYPlayerController::AbilityInputTagHeld);
	}
}

void ACYPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASC = GetPlayerState<ACYPlayerState>()->GetAbilitySystemComponent();
}

void ACYPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
	if (ASC)
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void ACYPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Red, *InputTag.ToString());
	if (ASC)
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void ACYPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Blue, *InputTag.ToString());
	if (ASC)
	{
		ASC->AbilityInputTagHeld(InputTag);
	}
}

void ACYPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		// add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACYPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACYPlayerController::Jump()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void ACYPlayerController::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

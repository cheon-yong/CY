// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CYPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CYPlayerState.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Equipment/CYEquipmentComponent.h"
#include "CY.H"

ACYPlayerCharacter::ACYPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACYPlayerCharacter::AddCharacterAbilities()
{
	if (ASC)
	{
		ASC->AddAbilities(StartAbilities);
	}
}

void ACYPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
}

void ACYPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
	UE_LOG(LogTemp, Log, TEXT("BeginPlay called on %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
	if (ACYPlayerState* CYPS = GetPlayerState<ACYPlayerState>())
	{
		ASC = CYPS->GetCYAbilitySystemComponent();
		ASC->InitAbilityActorInfo(CYPS, this);
		AddCharacterAbilities();

		OnInitASC.Broadcast(ASC);
		OnInitASC.Clear();

		for (const TObjectPtr<const UCYAbilitySet>& AbilitySet : StartAbilitySets)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/nullptr);
		}
	}
}

void ACYPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACYPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("BEGIN"));
	CY_LOG(LogCYNetwork, Log, TEXT("%s"), TEXT("END"));
	if (ACYPlayerState* CYPS = GetPlayerState<ACYPlayerState>())
	{
		ASC = CYPS->GetCYAbilitySystemComponent();
		ASC->InitAbilityActorInfo(CYPS, this);
		AddCharacterAbilities();

		OnInitASC.Broadcast(ASC);
		OnInitASC.Clear();

		for (const TObjectPtr<const UCYAbilitySet>& AbilitySet : StartAbilitySets)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/nullptr);
		}
	}
}

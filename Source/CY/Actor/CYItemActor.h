// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CYItemActor.generated.h"

class StaticMeshComponent;
class UCYItemInstance;
class USphereComponent;

UENUM(BlueprintType)
enum EItemState : uint8
{
	None		UMETA(DisplayName = "None"),
	Equipped	UMETA(DisplayName = "Equipped"),
	Dropped		UMETA(DisplayName = "Dropped"),
	MAX			UMETA(DisplayName = "MAX")
};

UCLASS()
class CY_API ACYItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACYItemActor();

	void Init(UCYItemInstance* InItemInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ItemState();

private:
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState = EItemState::None;

	TObjectPtr<USphereComponent> SphereComponent = nullptr;

	TObjectPtr<UCYItemInstance> ItemInstance;
};

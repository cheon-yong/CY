// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/IPickupable.h"
#include "Interaction/IInteractableTarget.h"
#include "Interaction/InteractionOption.h"

#include "CYItemActor.generated.h"

class USkeletalMeshComponent;
class UCYItemInstance;
class UCYItemDefinition;
class UBoxComponent;
class USceneComponent;

UENUM(BlueprintType)
enum EItemState : uint8
{
	None		UMETA(DisplayName = "None"),
	Equipped	UMETA(DisplayName = "Equipped"),
	Dropped		UMETA(DisplayName = "Dropped"),
	MAX			UMETA(DisplayName = "MAX")
};

UCLASS(BlueprintType, Blueprintable)
class CY_API ACYItemActor : public AActor, public IPickupable, public IInteractableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACYItemActor();

	virtual void Init(UCYItemInstance* InItemInstance);

	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnUse();

	void SetItemState(EItemState NewState);

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ItemState();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState = EItemState::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<UCYItemInstance> ItemInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCYItemDefinition> ItemDefinitionClass;

	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

};

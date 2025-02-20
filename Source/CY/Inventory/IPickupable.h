// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "IPickupable.generated.h"

class UCYItemDefinition;
class UCYItemInstance;
class UCYInventoryComponent;

USTRUCT(BlueprintType)
struct FPickupTemplate
{
	GENERATED_BODY()

	FPickupTemplate() {}

	FPickupTemplate(TSubclassOf<UCYItemDefinition> ItemDefinition, int Count)
		: ItemDef(ItemDefinition), StackCount(Count)
	{}
	
public:
	UPROPERTY(EditAnywhere)
	int32 StackCount = 1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCYItemDefinition> ItemDef;
};

USTRUCT(BlueprintType)
struct FPickupInstance
{
	GENERATED_BODY()
	
	FPickupInstance() { }
	
	FPickupInstance(TObjectPtr<UCYItemInstance> ItemInstace)
		: Item(ItemInstace)
	{}
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCYItemInstance> Item = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryPickup
{
	GENERATED_BODY()

	FInventoryPickup() {}
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPickupInstance> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPickupTemplate> Templates;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class IPickupable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual FInventoryPickup GetPickupInventory() const = 0;
};


/**  */
UCLASS()
class UPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UPickupableStatics();

public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<IPickupable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(UCYInventoryComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup);
};
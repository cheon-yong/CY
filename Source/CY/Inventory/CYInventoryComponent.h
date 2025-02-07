// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Inventory/CYItemEntry.h"

#include "CYInventoryComponent.generated.h"


class UCYItemInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CY_API UCYInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCYInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeComponent() override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(BlueprintCallable)
	UCYItemInstance* GetItem(int32 Index);

	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass);

	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UCYItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void AddItemInstanceToSlot(UCYItemInstance* InItemInstance, int32 Index);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UCYItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SwapItem(int32 a, int32 b);

	UFUNCTION(BlueprintCallable)
	TArray<UCYItemInstance*> GetAllItemInstances() const { return InventoryList.GetAllItemInstances(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxItemCount = 20;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UCYItemDefinition>> DefaultItems;

	// about widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxRow = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxColumn = 5;
	

private:
	UPROPERTY(Replicated)
	FCYItemList InventoryList;
};

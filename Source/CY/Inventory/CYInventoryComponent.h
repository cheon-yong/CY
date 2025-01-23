// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "CYInventoryComponent.generated.h"


class UCYItemInstance;

USTRUCT(BlueprintType)
struct FCYInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCYInventoryEntry() {}

	FString GetDebugString() const;
	
private:
	friend FCYInventoryList;
	friend UCYInventoryComponent;

	UPROPERTY()
	TObjectPtr<UCYItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};


USTRUCT(BlueprintType)
struct FCYInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FCYInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FCYInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	void AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass);
	void AddItem(UCYItemInstance* InItemInstance);
	void RemoveItem(UCYItemInstance* InItemInstance);
	TArray<FCYInventoryEntry> GetItemList() { return Entries; }

	TArray<UCYItemInstance*> GetAllItemInstances() const;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FCYInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};



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
	void AddItem(TSubclassOf<UCYItemDefinition> InItemDefinitionClass);

	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UCYItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UCYItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	TArray<UCYItemInstance*> GetAllItemInstances() const { return InventoryList.GetAllItemInstances(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UCYItemDefinition>> DefaultItems;

private:
	UPROPERTY(Replicated)
	FCYInventoryList InventoryList;
};

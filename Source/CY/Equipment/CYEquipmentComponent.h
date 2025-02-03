// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "CYEquipmentComponent.generated.h"

class UCYAbilitySystemComponent;


/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FCYAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCYAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FCYEquipmentList;
	friend UCYEquipmentComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UCYItemDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UCYItemInstance> Instance = nullptr;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FCYEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCYEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FCYEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FCYAppliedEquipmentEntry, FCYEquipmentList>(Entries, DeltaParms, *this);
	}

	UCYItemInstance* AddEntry(TSubclassOf<UCYItemDefinition> EquipmentDefinition);
	UCYItemInstance* AddEntry(UCYItemInstance* InItemInstance);
	void RemoveEntry(UCYItemInstance* Instance);

private:
	UCYAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UCYEquipmentComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FCYAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FCYEquipmentList> : public TStructOpsTypeTraitsBase2<FCYEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS(BlueprintType, Const)
class CY_API UCYEquipmentComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UCYEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UCYItemInstance* EquipItem(TSubclassOf<UCYItemDefinition> InItemDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UCYItemInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCYItemInstance* GetFirstInstanceOfType(TSubclassOf<UCYItemInstance> InstanceType);

	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UCYItemInstance*> GetEquipmentInstancesOfType(TSubclassOf<UCYItemInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FCYEquipmentList EquipmentList;

};

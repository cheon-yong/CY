// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PawnComponent.h"
#include "Inventory/CYItemEntry.h"

#include "CYEquipmentComponent.generated.h"

class UCYAbilitySystemComponent;
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
	UCYItemInstance* EquipItemInstance(UCYItemInstance* InItemInstance);

	void SpawnItemActor(UCYItemInstance* InItemInstance);

	void GiveAbilityToActor(UCYItemInstance* InItemInstance);

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
	TArray<UCYItemInstance*> GetEquipmentInstancesOfType(TSubclassOf<UCYItemInstance> InstanceType);

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

	UCYAbilitySystemComponent* GetAbilitySystemComponent();

private:
	UPROPERTY(Replicated)
	FCYItemList EquipmentList;
};

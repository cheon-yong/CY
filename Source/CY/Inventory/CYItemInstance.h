// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CYItemInstance.generated.h"

class UCYItemFragment;
class UCYItemDefinition;
struct FCYEquipmentActorToSpawn;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CY_API UCYItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UCYItemInstance();

	virtual void Init(TSubclassOf<UCYItemDefinition> InItemDefinitionClass, int32 InStackCount = 1);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UCYItemDefinition* GetItemDefinition() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UCYItemDefinition> GetItemDefinitionClass() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const UCYItemFragment* FindFragmentByClass(TSubclassOf<UCYItemFragment> FragmentClass) const;

	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;


	UFUNCTION(BlueprintPure, Category = Equipment)
	UObject* GetInstigator() const { return Instigator; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }


	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	virtual void OnEquipped();
	virtual void OnUnequipped();

	virtual void SpawnEquipmentActors(const TArray<FCYEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

private:
	UFUNCTION()
	void OnRep_Instigator();

public:
	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 0;

private:
	UPROPERTY(Replicated)
	TSubclassOf<UCYItemDefinition> ItemDefinitionClass;

	UPROPERTY(ReplicatedUsing = OnRep_Instigator)
	TObjectPtr<UObject> Instigator;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

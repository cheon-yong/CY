// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CYItemInstance.generated.h"

class UCYItemFragment;
class UCYItemDefinition;
/**
 * 
 */
UCLASS()
class CY_API UCYItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UCYItemInstance();

	virtual void Init(TSubclassOf<UCYItemDefinition> InItemDefinitionClass);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UCYItemDefinition* GetItemDefinition() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const UCYItemFragment* FindFragmentByClass(TSubclassOf<UCYItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	virtual void OnEquipped();
	virtual void OnUnequipped();

private:
	UPROPERTY(Replicated)
	TSubclassOf<UCYItemDefinition> ItemDefinitionClass;
};

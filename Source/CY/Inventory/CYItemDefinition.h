// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CYItemDefinition.generated.h"

class UCYItemInstance;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class CY_API UCYItemFragment : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInstanceCreated(UCYItemInstance* Instance) const {}
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class CY_API UCYItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UCYItemDefinition();

	const TObjectPtr<UCYItemFragment> FindFragmentByClass(TSubclassOf<UCYItemFragment> FragmentClass) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FName ItemName;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, Instanced)
	TArray<TObjectPtr<UCYItemFragment>> Fragments;
};


//@TODO: Make into a subsystem instead?
UCLASS()
class UCYInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = FragmentClass))
	static const UCYItemFragment* FindItemDefinitionFragment(TSubclassOf<UCYItemDefinition> ItemDef, TSubclassOf<UCYItemFragment> FragmentClass);
};

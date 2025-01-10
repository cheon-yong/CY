// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CYItemInstance.generated.h"

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
	TSubclassOf<UCYItemDefinition> GetItemDefinition() const;

private:
	UPROPERTY(Replicated)
	TSubclassOf<UCYItemDefinition> ItemDefinitionClass;
};

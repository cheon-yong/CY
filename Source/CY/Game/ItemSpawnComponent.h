// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameStateComponent.h"
#include "ItemSpawnComponent.generated.h"

class UFieldItemData;
class ACYSection;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitialize);

/**
 * 
 */
UCLASS()
class CY_API UItemSpawnComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	UItemSpawnComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	void Init();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void GetAllSectionInWorld();

	void CalculateItemRatio();

	void SpawnItems();

	UPROPERTY()
	uint8 bInitialized : 1;

	UPROPERTY(BlueprintAssignable)
	FOnInitialize OnInitialize;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFieldItemData> FieldItemData;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<ACYSection>> Sections;

};

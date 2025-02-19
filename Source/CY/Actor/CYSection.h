// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CYGameplayTags.h"

#include "CYSection.generated.h"

class UBoxComponent;
class UCYItemInstance;
class ACYItemSpawnPoint;
class UCYItemDefinition;

UCLASS(BlueprintType, Blueprintable)
class ACYSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACYSection();


public:
	void Init();

	void SetOrAddItemDefWithCount(TSubclassOf<UCYItemDefinition> ItemDef, int32 Count);

	UFUNCTION()
	void SpawnItemToPoint();

	TObjectPtr<ACYItemSpawnPoint> GetRandomSpawnPoint();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void GetAllSpawnPoints();
	TSubclassOf<UCYItemDefinition> GetRandomItemDefByTag(FGameplayTag ItemType);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Area;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<UCYItemDefinition>, int32> ItemDefToCreate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<UCYItemInstance>, int32> ItemsInSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ACYItemSpawnPoint>> SpawnPoints;
};

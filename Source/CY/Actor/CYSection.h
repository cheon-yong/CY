// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Area;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<TObjectPtr<UCYItemDefinition>, int32> ItemDefToCreate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<UCYItemInstance>, int32> ItemsInSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ACYItemSpawnPoint>> SpawnPoints;
};

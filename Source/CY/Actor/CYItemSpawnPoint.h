// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CYItemSpawnPoint.generated.h"

class USphereComponent;


UCLASS(BlueprintType, Blueprintable)
class CY_API ACYItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACYItemSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent = nullptr;
};

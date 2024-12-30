// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CYProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class CY_API ACYProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACYProjectile();

	virtual void Tick(float DeltaTime) override;

	TObjectPtr<UStaticMeshComponent> GetStaticMesh() { return SMC; }
	TObjectPtr<UProjectileMovementComponent> GetProjectileMovement() { return PMC; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void DestroyProjectile();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float LifeTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UStaticMeshComponent> SMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UProjectileMovementComponent> PMC;
};

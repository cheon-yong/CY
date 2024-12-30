// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "CYProjectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UGameplayEffect;
class UAbilitySystemComponent;
class UCYAbilitySystemComponent;

UCLASS()
class CY_API ACYProjectile : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACYProjectile();

	TObjectPtr<UStaticMeshComponent> GetStaticMesh() { return SMC; }
	TObjectPtr<UProjectileMovementComponent> GetProjectileMovement() { return PMC; }
	TObjectPtr<UBoxComponent> GetBoxComponent() { return BoxComponent; }

	void SetGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);

	void InitProjectile(TObjectPtr<APawn> Instigator);

	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	UFUNCTION(BlueprintCallable)
	UCYAbilitySystemComponent* GetCYAbilitySystemComponent() const { return ASC; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void ApplyEffectToTarget(AActor* Target);
	void InvokeGameplayCue(AActor* Target);

	// Not Use
	UFUNCTION()
  	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void DestroyProjectile();

protected:
	UPROPERTY()
	TObjectPtr<UCYAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float LifeTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UStaticMeshComponent> SMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UProjectileMovementComponent> PMC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CYProjectile.h"

#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystem/CYAbilitySystemComponent.h"
#include "TimerManager.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
ACYProjectile::ACYProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SMC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ASC = CreateDefaultSubobject<UCYAbilitySystemComponent>(TEXT("ASC"));

	RootComponent = BoxComponent;
	SMC->SetupAttachment(BoxComponent);

	bReplicates = true;
	BoxComponent->SetIsReplicated(true);
	PMC->SetIsReplicated(true);
	SMC->SetIsReplicated(true);
}

void ACYProjectile::SetGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	AttackDamageEffect = GameplayEffect;
}

void ACYProjectile::InitProjectile(TObjectPtr<APawn> InInstigator)
{
	SetInstigator(InInstigator);
	ASC->InitAbilityActorInfo(InInstigator, this);
	PMC->bRotationFollowsVelocity = true;
}

void ACYProjectile::NotifyActorBeginOverlap(AActor* Other)
{
	if (GetInstigator() == Other)
		return;

	Super::NotifyActorBeginOverlap(Other);

	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);

	SMC->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	DestroyProjectile();
}

UAbilitySystemComponent* ACYProjectile::GetAbilitySystemComponent() const
{
	return GetCYAbilitySystemComponent();
}

void ACYProjectile::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(AttackDamageEffect, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void ACYProjectile::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}

void ACYProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*InvokeGameplayCue(OtherActor);
	ApplyEffectToTarget(OtherActor);

	SMC->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	DestroyProjectile();*/
}


// Called when the game starts or when spawned
void ACYProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle LifeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle, 
		this, 
		&ThisClass::DestroyProjectile,
		LifeTime, 
		false
	);
}

void ACYProjectile::DestroyProjectile()
{
	SMC->SetHiddenInGame(true);
	PMC->Deactivate();

	Destroy();
}


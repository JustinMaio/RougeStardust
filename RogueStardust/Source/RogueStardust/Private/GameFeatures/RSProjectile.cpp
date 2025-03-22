// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/RSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARSProjectile::ARSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARSProjectile::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(LifetimeTimerHandle, this, &ARSProjectile::OnProjectileFinished, ProjectileLifetime, false);
}

// Called every frame
void ARSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARSProjectile::OnProjectileFinished()
{
	Destroy();
}

void ARSProjectile::InitShotDirection(const FVector& shotDirection)
{
	if (UProjectileMovementComponent* projectileMovement = FindComponentByClass<UProjectileMovementComponent>())
	{
		projectileMovement->Velocity = shotDirection * projectileMovement->InitialSpeed;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSProjectile.generated.h"

UCLASS()
class ROGUESTARDUST_API ARSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSProjectile();

	virtual void Tick(float DeltaTime) override;
	void OnProjectileFinished();
	void InitShotDirection(const FVector& shotDirection);
	
	UPROPERTY(EditAnywhere)
	float ProjectileLifetime = 5.0f;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 25.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	FTimerHandle LifetimeTimerHandle;
};

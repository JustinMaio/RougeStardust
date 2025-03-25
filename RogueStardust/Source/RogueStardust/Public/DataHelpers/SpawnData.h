// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RSEnemyShip.h"
#include "GameFeatures/Splines/RSEnemySpline.h"
#include "SpawnData.generated.h"

/**
 * 
 */
USTRUCT()
struct ROGUESTARDUST_API FSpawnData
{
	GENERATED_BODY()
public:
	//might swithc this to a class to spawn instead of an object
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARSEnemyShip> ShipClassToSpawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ARSEnemySpline> AssignedSpline;

	UPROPERTY(EditAnywhere)
	FVector SpawnOffset = FVector::ZeroVector;
};

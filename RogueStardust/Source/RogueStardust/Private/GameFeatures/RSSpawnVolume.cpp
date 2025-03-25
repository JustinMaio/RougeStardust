// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/RSSpawnVolume.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

ARSSpawnVolume::ARSSpawnVolume() : Super()
{
	OnActorBeginOverlap.AddDynamic(this, &ARSSpawnVolume::OnOverlapBegin);
}

void ARSSpawnVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* otherActor)
{
	UWorld* World = GetWorld();
	if (World && otherActor && (otherActor != this))
	{
		APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (otherActor == player && m_HasTriggered == false)
		{
			for (auto SpawnIter = SpawnData.begin(); SpawnIter != SpawnData.end(); ++SpawnIter)
			{
				FSpawnData currentSpawnData = (*SpawnIter);
				//Spawn Assigned Actors here
				FTransform SpawnTransform;
				FVector SpawnLocation = currentSpawnData.SpawnOffset;
				USplineComponent* spline = nullptr;
				if (currentSpawnData.AssignedSpline)
				{
					spline = currentSpawnData.AssignedSpline->GetSplineComponent();
					if (spline)
					{
						SpawnLocation = spline->GetWorldLocationAtDistanceAlongSpline(0);
						//claculate offset here too using the correct vecors
						//const FVector UpDirection = spline->GetUpVectorAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local);
						//const FVector RightDirection = spline->GetRightVectorAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local);
					}
				}

				SpawnTransform.SetLocation(SpawnLocation);
				ARSEnemyShip* newEnemy = World->SpawnActor<ARSEnemyShip>(currentSpawnData.ShipClassToSpawn, SpawnTransform);
				if (newEnemy)
				{
					if (spline)
					{
						newEnemy->InitEnemySpline(spline);
					}
				}
			}
			m_HasTriggered = true;
		}
	}
}
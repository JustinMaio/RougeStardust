// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RSEnemyShip.h"
#include "Managers/SplineManager.h"
#include "Components/SplineComponent.h"

ARSEnemyShip::ARSEnemyShip() : Super()
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ARSEnemyShip::InitEnemySpline(USplineComponent* assignedSpline)
{
	if (assignedSpline)
	{
		m_AssignedSpline = assignedSpline;
	}
}

void ARSEnemyShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (m_AssignedSpline)
    {
        float SplineUpdateTime = ShipSpeed * DeltaTime;
        float updatedSplineDist = SplineUpdateTime + splineDist;
        FVector splinePos = m_AssignedSpline->GetWorldLocationAtDistanceAlongSpline(updatedSplineDist);
        FVector oldSplinePos = m_AssignedSpline->GetWorldLocationAtDistanceAlongSpline(splineDist);
        FVector splineHeading = splinePos - oldSplinePos;
        splineHeading.Normalize();
        FRotator facing = splineHeading.Rotation();
        SetActorRotation(facing);

        const FVector UpDirection = m_AssignedSpline->GetUpVectorAtDistanceAlongSpline(updatedSplineDist, ESplineCoordinateSpace::Local);
        const FVector RightDirection = m_AssignedSpline->GetRightVectorAtDistanceAlongSpline(updatedSplineDist, ESplineCoordinateSpace::Local);

        SetActorLocation(splinePos);

        splineDist = updatedSplineDist;
        if (splineDist > m_AssignedSpline->GetSplineLength())
        {
            //will do something else later maybe
            Destroy();
        }
    }
}
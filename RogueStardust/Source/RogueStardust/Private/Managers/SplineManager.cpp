// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SplineManager.h"
#include "Components/SplineComponent.h"


void USplineManager::RegisterSpline(ERSSplineType splineType, USplineComponent* spline)
{
	SplineMap.FindOrAdd(splineType, spline);
}
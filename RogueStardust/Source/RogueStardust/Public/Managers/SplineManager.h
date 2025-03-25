// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFeatures/Splines/RSSpline.h"
#include "SplineManager.generated.h"

class USplineComponent;

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API USplineManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const { return true; }

	//making public for now but I'll create an interface later also might want to be an array of arrays
	TMap<ERSSplineType, TObjectPtr<USplineComponent>> SplineMap;

	UFUNCTION(BlueprintCallable)
	void RegisterSpline(ERSSplineType splineType, USplineComponent* spline);
	
};

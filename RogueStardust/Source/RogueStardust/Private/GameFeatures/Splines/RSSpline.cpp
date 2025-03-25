// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Splines/RSSpline.h"
#include "Camera/CameraComponent.h"
#include "Managers/SplineManager.h"
#include "Components/SplineComponent.h"


// Sets default values
ARSSpline::ARSSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

// Called when the game starts or when spawned
void ARSSpline::BeginPlay()
{
	Super::BeginPlay();

    if (USplineManager* splineMgr = GetWorld()->GetSubsystem<USplineManager>())
    {
        splineMgr->RegisterSpline(SplineType, SplineComp);
    }
}

// Called every frame
void ARSSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/RSSpline.h"
#include "Characters/RSPlayerShip.h"
#include "Camera/CameraComponent.h"
#include "Managers/SplineManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARSSpline::ARSSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

    //Attach our components
    //This should actually attach to the spline  and not the ship it feels awkward with it attach directly to the ship
    SpringArmComp->SetupAttachment(SplineComp);
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    //Assign SpringArm class variables.
    if (SplineComp)
    {
        FVector SplineStart = SplineComp->GetWorldLocationAtDistanceAlongSpline(0);
        SpringArmComp->SetRelativeLocationAndRotation(CameraOffset + SplineStart, FRotator(0.0f, 0.0f, 0.0f));
        SpringArmComp->TargetArmLength = 400.f;
        SpringArmComp->bEnableCameraLag = true;
        SpringArmComp->CameraLagSpeed = 3.0f;
    }

}

// Called when the game starts or when spawned
void ARSSpline::BeginPlay()
{
	Super::BeginPlay();

    if (USplineManager* splineMgr = GetWorld()->GetSubsystem<USplineManager>())
    {
        splineMgr->RegisterSpline(SplineType, SplineComp);
    }
	

    if (SplineComp && SplineType == ERSSplineType::E_PlayerSpline)
    {
        //Might want to fetch the Player for Reference
        if (ARSPlayerShip* player = Cast<ARSPlayerShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
        {
            if (APlayerController* playerController = Cast<APlayerController>(player->GetController()))
            {
                playerController->SetViewTarget(this);
            }
            SpringArmComp->SetRelativeLocationAndRotation(CameraOffset + player->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
        }
    }
}

// Called every frame
void ARSSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (SplineComp && SplineType == ERSSplineType::E_PlayerSpline)
    {
        //Might want to fetch the Player for Reference
        if (ARSPlayerShip* player = Cast<ARSPlayerShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
        {
            float SplineUpdateTime = player->ShipSpeed * DeltaTime;
            float updatedSplineDist = SplineUpdateTime + splineDist;
            FVector splinePos = SplineComp->GetWorldLocationAtDistanceAlongSpline(updatedSplineDist);
            FVector oldSplinePos = SplineComp->GetWorldLocationAtDistanceAlongSpline(splineDist);
            FVector splineHeading = splinePos - oldSplinePos;
            splineHeading.Normalize();
            FRotator facing = splineHeading.Rotation();
            SpringArmComp->SetWorldRotation(facing);
            SpringArmComp->SetWorldLocation(splinePos);
            splineDist = updatedSplineDist;
        }
    }

}


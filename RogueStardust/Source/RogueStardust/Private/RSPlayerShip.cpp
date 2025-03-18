// Fill out your copyright notice in the Description page of Project Settings.
#include "RSPlayerShip.h"

#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "Managers/SplineManager.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// Sets default values
ARSPlayerShip::ARSPlayerShip() : Super()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

    //Attach our components
    GetMesh()->SetupAttachment(RootComponent);
    SpringArmComp->SetupAttachment(GetMesh());
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    //Assign SpringArm class variables.
    SpringArmComp->SetRelativeLocationAndRotation(FVector(-10.0f, -10.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
    SpringArmComp->TargetArmLength = 400.f;
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 3.0f;
}
  
UAbilitySystemComponent* ARSPlayerShip::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARSPlayerShip::BeginPlay()
{
	Super::BeginPlay();

    //Bind all the input to the player 

    splineDist = 0.0f;
    SplineOffset = FVector::ZeroVector;
    //Store and offset of the distances away from the spline
    //Also run through the spline via time and use that to set the distance
    USplineManager* splineMgr = GetWorld()->GetSubsystem<USplineManager>();
    if (splineMgr)
    {
        USplineComponent* playerSpline = splineMgr->SplineMap[E_PlayerSpline];
        if (playerSpline)
        {
            levelSpline = playerSpline;
        }
    }
    //TEMP CAMERA attache code
    // Create a dummy root component we can attach things to.
    //if (RootComponent == nullptr)
    //{
    //    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    //}
    //// Create a camera and a visible object
    //UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    //// Attach our camera and visible object to our root component. Offset and rotate the camera.
    //OurCamera->SetupAttachment(RootComponent);
    //OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
    //OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    //GetMesh()->SetupAttachment(RootComponent);
	
}

// Called every frame
void ARSPlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (levelSpline)
    {
        float SplineUpdateTime = ShipSpeed * DeltaTime;
        float updatedSplineDist = SplineUpdateTime + splineDist;
        FVector splinePos = levelSpline->GetWorldLocationAtDistanceAlongSpline(updatedSplineDist);
        FVector oldSplinePos = levelSpline->GetWorldLocationAtDistanceAlongSpline(splineDist);
        FVector splineHeading = splinePos - oldSplinePos;
        splineHeading.Normalize();
        FRotator facing = splineHeading.Rotation();
        SetActorRotation(facing);
        if (SavedModifiedOffset.IsZero())
        {
            SetActorLocation(splinePos);
        }
        else
        {
            const FVector UpDirection = this->GetActorUpVector();
            const FVector RightDirection = this->GetActorRightVector();
            FVector UpMod = UpDirection * SavedModifiedOffset.Y;
            FVector RightMod = RightDirection * SavedModifiedOffset.X;
            SetActorLocation(splinePos + UpMod + RightMod);
        }

        splineDist = updatedSplineDist;

        if (Controller)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            // get forward vector
            const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            const FVector UpDirection = this->GetActorUpVector();
            const FVector RightDirection = this->GetActorRightVector();
            // get right vector 
            //const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            if (!SplineOffset.IsZero())
            {
                FVector modifiedPos;
                FVector UpMod = UpDirection * SplineOffset.Z;// *ShipSpeed;// *DeltaTime;
                FVector RightMod = RightDirection * SplineOffset.Y;// *ShipSpeed;// *DeltaTime;
                modifiedPos = UpMod + RightMod;
                FVector modifiedPosOffset = splinePos + (modifiedPos * ShipSpeed);
                //do this unless the over the distance away from the spline position
                double DistFromSpline = FVector::DistSquared(splinePos, modifiedPosOffset);
                if (DistFromSpline < FMath::Square(MaxOffsetDistance))
                {
                    SplineModifiedOffset += (modifiedPos * ShipSpeed);
                    SavedModifiedOffset.X += SplineOffset.Y * ShipSpeed;
                    SavedModifiedOffset.Y += SplineOffset.Z * ShipSpeed;
                    FVector newOffsetPos = splinePos + SplineModifiedOffset;
                    SetActorLocation(newOffsetPos);
                    SplineOffset = FVector::ZeroVector;
                   // SplineModifiedOffset = FVector::ZeroVector;
                }
            }
        }
    }

}

void ARSPlayerShip::ShootPrimaryWeapon(const FInputActionValue& Value)
{
    bool shotPressed = Value.Get<bool>();
    UWorld* World = GetWorld();
    FTimerManager& TimerManager = World->GetTimerManager();
    if (shotPressed && !TimerManager.IsTimerActive(ShotDelayHandle))
    {
        //Spawn a shot here
        // 
        FTransform spawnLocation;
        spawnLocation.SetLocation(GetActorLocation());
        //have the class more custom and itself will manage the lifetime
        if (ProjectileClass)
        {
            //might need to do some init code here to shoot in the correct direction and speed
            World->SpawnActor<AActor>(ProjectileClass, spawnLocation);
        }
        //will change it up for charge shot
        TimerManager.SetTimer(ShotDelayHandle, this, &ARSPlayerShip::OnShotDelayDone, shotDelayTime, false);

    }
}

void ARSPlayerShip::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();
    MovementVector.Normalize();
    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector UpDirection = this->GetActorUpVector();
        // get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        //lastStoredInputs.X = MovementVector.X;
        //lastStoredInputs.Y = MovementVector.Y;
        SplineOffset.Y = MovementVector.Y;//Right Vector
        SplineOffset.Z = MovementVector.X;//upVector
        // add movement 
        //AddMovementInput(UpDirection, MovementVector.Y * ShipSpeed);
        //AddMovementInput(RightDirection, MovementVector.X * ShipSpeed);
    }
}

void ARSPlayerShip::MoveReleased(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();
    MovementVector.Normalize();

    if (Controller != nullptr)
    {
        SplineOffset.Y = MovementVector.Y;
        SplineOffset.Z = MovementVector.X;
    }
}

void ARSPlayerShip::OnShotDelayDone()
{
    UWorld* World = GetWorld();
    FTimerManager& TimerManager = World->GetTimerManager();
    TimerManager.ClearTimer(ShotDelayHandle);
}

// Called to bind functionality to input
void ARSPlayerShip::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ARSPlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
    {
        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARSPlayerShip::Move);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARSPlayerShip::MoveReleased);


        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ARSPlayerShip::ShootPrimaryWeapon);
    }
    else
    {
        //UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}


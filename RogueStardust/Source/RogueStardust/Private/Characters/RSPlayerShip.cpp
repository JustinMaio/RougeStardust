// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/RSPlayerShip.h"

#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "Managers/SplineManager.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFeatures/RSProjectile.h"
#include "GameFeatures/Splines/RSSpline.h"

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
        USplineComponent* playerSpline = splineMgr->SplineMap[ERSSplineType::E_PlayerSpline];
        if (playerSpline)
        {
            levelSpline = playerSpline;
        }
    }
    TArray<UStaticMeshComponent*> staticMeshArray;
    this->GetComponents(staticMeshArray, true);
    for (auto iter = staticMeshArray.begin(); iter != staticMeshArray.end(); ++iter)
    {
        if ((*iter))
        {
            //Change this later to something more appropiate
            if ((*iter)->GetName() == "Cube")
            {
                MeshComp = (*iter);
            }
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
         
        const FVector UpDirection = levelSpline->GetUpVectorAtDistanceAlongSpline(updatedSplineDist, ESplineCoordinateSpace::Local);
        const FVector RightDirection = levelSpline->GetRightVectorAtDistanceAlongSpline(updatedSplineDist, ESplineCoordinateSpace::Local);

        if (SavedModifiedOffset.IsZero())
        {
            SetActorLocation(splinePos);
        }
        else
        {
            FVector UpMod = UpDirection * SavedModifiedOffset.Y;
            FVector RightMod = RightDirection * SavedModifiedOffset.X;
            SetActorLocation(splinePos + UpMod + RightMod);
        }

        splineDist = updatedSplineDist;

        if (Controller)
        {
            // there is a edge case where if we moved to far that we can't accept new input so we'll have to correct that
            if (!SplineOffset.IsZero())
            {
                FVector modifiedPos;
                FVector UpMod = UpDirection * SplineOffset.Z;
                FVector RightMod = RightDirection * SplineOffset.Y;
                modifiedPos = UpMod + RightMod;
                //FVector modifiedPosOffset = splinePos + (modifiedPos * ShipSpeed * DeltaTime);
                FVector modifiedPosOffset = splinePos + SavedModifiedOffset + (modifiedPos * ShipSpeed * DeltaTime);
                //do this unless the over the distance away from the spline position
                double DistFromSpline = FVector::DistSquared(splinePos, modifiedPosOffset);
                if (DistFromSpline < FMath::Square(MaxOffsetDistance))
                {
                    SplineModifiedOffset += (modifiedPos * ShipSpeed * DeltaTime);
                    SavedModifiedOffset.X += SplineOffset.Y * ShipSpeed * DeltaTime;
                    SavedModifiedOffset.Y += SplineOffset.Z * ShipSpeed * DeltaTime;
                    FVector newOffsetPos = splinePos + SplineModifiedOffset;
                    SplineOffset = FVector::ZeroVector;
                }
            }
        }
    }

    //Update Roll Reset
    if (MeshComp && ToOrigRollRate > 0 && ToOrigRollRate < 1.0f)
    {
        FRotator curRotation = MeshComp->GetRelativeRotation();
        double angle = FMath::Lerp(curRotation.Roll, 0, ToOrigRollRate);
        curRotation.Roll = angle;
        MeshComp->SetRelativeRotation(curRotation);
        ToOrigRollRate += RollRate;
        ToOrigRollRate = FMath::Clamp(ToOrigRollRate, 0.0f, 1.0f);
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
            ARSProjectile* projectile = World->SpawnActor<ARSProjectile>(ProjectileClass, spawnLocation);
            if (projectile)
            {
                projectile->InitShotDirection(GetActorForwardVector());
            }
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

void ARSPlayerShip::OnDeflectWindowDone()
{
    UWorld* World = GetWorld();
    FTimerManager& TimerManager = World->GetTimerManager();
    TimerManager.ClearTimer(DeflectDetectionHandle);
}

void ARSPlayerShip::OnActiveDeflectonDone()
{
    UWorld* World = GetWorld();
    FTimerManager& TimerManager = World->GetTimerManager();
    TimerManager.ClearTimer(DeflectActiveHandle);
    //hide the shield object (perhaps make a shield component for other things too)
}

void ARSPlayerShip::RollPressed(const FInputActionValue& Value)
{
    float scalar = Value.Get<float>();
    scalar = FMath::Clamp(scalar, -1.0, 1.0);
    if (Controller != nullptr)
    {
        if (LastRollStatus < 0 && scalar > 0 ||
            LastRollStatus > 0 && scalar < 0)
        {
            InterpolateRollRate = 0.0f;
        }
        LastRollStatus = scalar;
        //rotate roll by 85-90 degrees roughly
        //TODO: if swapping between both roll states (negative to postive without letting go) need to handle that edge case
        if (MeshComp)
        {
            ToOrigRollRate = 0.0f;
            FRotator curRotation = MeshComp->GetRelativeRotation();
            double angle = FMath::Lerp(curRotation.Roll, scalar * 85.0, InterpolateRollRate);
            curRotation.Roll = angle;
            MeshComp->SetRelativeRotation(curRotation);
            InterpolateRollRate += RollRate;//Might want to factor in Delta time here
            InterpolateRollRate = FMath::Clamp(InterpolateRollRate, 0.0f, 1.0f);
        }
        UWorld* World = GetWorld();
        FTimerManager& TimerManager = World->GetTimerManager();
        if (TimerManager.IsTimerActive(DeflectDetectionHandle))
        {
            //for now show a shield/sphere or something so we can see it test something else later
            TimerManager.SetTimer(DeflectActiveHandle, this, &ARSPlayerShip::OnActiveDeflectonDone, DeflectActiveTime, false);

        }
    }
}

void ARSPlayerShip::RollReleased(const FInputActionValue& Value)
{
    float scalar = Value.Get<float>();
    scalar = FMath::Clamp(scalar, -1.0, 1.0);
    if (Controller != nullptr)
    {
        InterpolateRollRate = 0.0f;
        //rotate roll by 85-90 degrees roughly
        //need to claculate the angle we are currently for roll and interpolate from there
        if (MeshComp)
        {
            ToOrigRollRate += RollRate;
            ToOrigRollRate = FMath::Clamp(ToOrigRollRate, 0.0f, 1.0f);
        }
        //might do this later by frames and not the timer later
        UWorld* World = GetWorld();
        FTimerManager& TimerManager = World->GetTimerManager();
        TimerManager.SetTimer(DeflectDetectionHandle, this, &ARSPlayerShip::OnShotDelayDone, DeflectWindowTime, false);

    }
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

        //Actions
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ARSPlayerShip::ShootPrimaryWeapon);
        EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ARSPlayerShip::RollPressed);
        EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &ARSPlayerShip::RollReleased);
    }
    else
    {
        //UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}


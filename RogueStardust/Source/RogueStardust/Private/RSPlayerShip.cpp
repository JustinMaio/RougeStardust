// Fill out your copyright notice in the Description page of Project Settings.
#include "RSPlayerShip.h"

#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARSPlayerShip::ARSPlayerShip()
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

}

void ARSPlayerShip::ShootPrimaryWeapon()
{

}

void ARSPlayerShip::MoveYAxis(float axisValue)
{
    //hard position changes for testing purposes
    FVector curPosition = GetActorLocation();
    if (axisValue > 0.0f)
    {
        curPosition.Z += 100.0f;
        SetActorLocation(curPosition);
    }
    else if (axisValue < 0.0f)
    {
        curPosition.Z -= 100.0f;
        SetActorLocation(curPosition);
    }
}

void ARSPlayerShip::MoveXAxis(float axisValue)
{

}


// Called to bind functionality to input
void ARSPlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Respond when our "Grow" key is pressed or released.
    InputComponent->BindAction("Fire", IE_Pressed, this, &ARSPlayerShip::ShootPrimaryWeapon);

    // Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
    InputComponent->BindAxis("MoveX", this, &ARSPlayerShip::MoveXAxis);
    InputComponent->BindAxis("MoveY", this, &ARSPlayerShip::MoveYAxis);
}


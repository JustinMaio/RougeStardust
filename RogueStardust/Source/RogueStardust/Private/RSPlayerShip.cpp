// Fill out your copyright notice in the Description page of Project Settings.
#include "RSPlayerShip.h"

#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARSPlayerShip::ARSPlayerShip()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
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
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // Create a camera and a visible object
    UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    // Attach our camera and visible object to our root component. Offset and rotate the camera.
    OurCamera->SetupAttachment(RootComponent);
    OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
    OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    GetMesh()->SetupAttachment(RootComponent);
	
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


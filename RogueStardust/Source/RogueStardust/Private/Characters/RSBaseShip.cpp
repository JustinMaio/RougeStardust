// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RSBaseShip.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARSBaseShip::ARSBaseShip()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARSBaseShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSBaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ARSBaseShip::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


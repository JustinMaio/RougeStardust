// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/Splines/RSSpline.h"
#include "RSPlayerSpline.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API ARSPlayerSpline : public ARSSpline
{
	GENERATED_BODY()

public:
	ARSPlayerSpline();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//used for the player spline only
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;

	float splineDist = 0.0f;
	FVector CameraOffset = FVector(-10.0f, -10.0f, 0.0f);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSSpline.generated.h"

class USplineComponent;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ERSSplineType : uint8
{
	E_PlayerSpline = 0,
	E_EnemySpline,
	E_SplineMax
};

UCLASS()
class ROGUESTARDUST_API ARSSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSSpline();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	ERSSplineType SplineType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Cane be used with anything
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComp;

	//used for the player spline only
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;

	float splineDist = 0.0f;
	FVector CameraOffset = FVector(-10.0f, -10.0f, 0.0f);

};

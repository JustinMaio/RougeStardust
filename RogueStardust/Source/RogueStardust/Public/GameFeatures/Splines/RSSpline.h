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

	USplineComponent* GetSplineComponent() { return SplineComp; }

	UPROPERTY(EditAnywhere)
	ERSSplineType SplineType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Cane be used with anything
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComp;

private:

};

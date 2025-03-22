// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/RSBaseShip.h"
#include "AbilitySystemInterface.h"
#include "RSPlayerShip.generated.h"

class UAbilitySystemComponent;
class USplineComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ROGUESTARDUST_API ARSPlayerShip : public ARSBaseShip
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSPlayerShip();

	UPROPERTY(EditAnywhere)
	float ShipSpeed = 90.0f;

	UPROPERTY(EditAnywhere)
	float RollRate = 0.1f;

	UPROPERTY(EditAnywhere)
	float MaxOffsetDistance = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ShootPrimaryWeapon(const FInputActionValue& Value);
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void MoveReleased(const FInputActionValue& Value);

	void OnShotDelayDone();
	void OnDeflectWindowDone();
	void OnActiveDeflectonDone();
	void RollPressed(const FInputActionValue& Value);
	void RollReleased(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<USplineComponent> levelSpline;
	float splineDist = 25.0f;
	FVector SplineOffset = FVector::ZeroVector;
	FVector SplineModifiedOffset = FVector::ZeroVector;
	FVector SavedModifiedOffset = FVector::ZeroVector;

	float shotDelayTime = 0.2f;
	FTimerHandle ShotDelayHandle;

	float InterpolateRollRate = 0.0f;
	float ToOrigRollRate = 0.0f;
	float LastRollStatus = 0.0f;

	float DeflectWindowTime = 0.5f;
	float DeflectActiveTime = 1.5f;
	FTimerHandle DeflectDetectionHandle;
	FTimerHandle DeflectActiveHandle;


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComp;

};

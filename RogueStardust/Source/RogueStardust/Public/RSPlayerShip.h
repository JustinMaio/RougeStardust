// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RSPlayerShip.generated.h"

class UAbilitySystemComponent;
class USplineComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

UCLASS()
class ROGUESTARDUST_API ARSPlayerShip : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSPlayerShip();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


	UPROPERTY(EditAnywhere)
	float ShipSpeed = 40.0f;

	UPROPERTY(EditAnywhere)
	float MaxOffsetDistance = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

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

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<USplineComponent> levelSpline;
	float splineDist = 0.0f;
	FVector SplineOffset = FVector::ZeroVector;
	FVector SplineModifiedOffset = FVector::ZeroVector;
	FVector SavedModifiedOffset = FVector::ZeroVector;

	float shotDelayTime = 0.2f;
	FTimerHandle ShotDelayHandle;

};

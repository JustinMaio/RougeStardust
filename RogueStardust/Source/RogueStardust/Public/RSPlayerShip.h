// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RSPlayerShip.generated.h"

class UAbilitySystemComponent;

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



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ShootPrimaryWeapon();
	void MoveYAxis(float axisValue);
	void MoveXAxis(float axisValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

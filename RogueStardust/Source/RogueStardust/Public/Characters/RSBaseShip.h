// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RSBaseShip.generated.h"

class UAbilitySystemComponent;

UCLASS()
class ROGUESTARDUST_API ARSBaseShip : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSBaseShip();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ShipSpeed = 90.0f;

protected:

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float splineDist = 0.0f;
};

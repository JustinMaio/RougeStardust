// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RSBaseShip.h"
#include "RSEnemyShip.generated.h"


class UAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API ARSEnemyShip : public ARSBaseShip
{
	GENERATED_BODY()

public:
	ARSEnemyShip();
	void InitEnemySpline(USplineComponent* assignedSpline);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<USplineComponent> m_AssignedSpline;
};

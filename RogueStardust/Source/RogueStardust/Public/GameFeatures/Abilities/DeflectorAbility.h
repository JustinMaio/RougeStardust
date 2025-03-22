// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/Abilities/RSGameplayAbility.h"
#include "DeflectorAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API UDeflectorAbility : public URSGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float DeflectWindowTime = 0.5f;

	UPROPERTY(EditAnywhere)
	float DeflectActiveTime = 1.5f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


private:
	
	FTimerHandle DeflectDetectionHandle;
	FTimerHandle DeflectActiveHandle;
};

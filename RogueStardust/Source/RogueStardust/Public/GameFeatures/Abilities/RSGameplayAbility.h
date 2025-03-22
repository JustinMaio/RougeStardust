// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Helpers/RSGASHelpers.h"
#include "RSGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API URSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Ability")
	ERSAbilityInputID AbilityInputID = ERSAbilityInputID::None;

	ERSAbilityInputID GetAbilityInputID() { return AbilityInputID; }
};

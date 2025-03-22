// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/Abilities/RSBaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RSHealthAttributeSet.generated.h"

//will change some of these labels later
DECLARE_MULTICAST_DELEGATE_FourParams(FDamageTakenEvent, AActor* /*Effect Instigator*/, AActor* /*EffectCauser*/, const FGameplayTagContainer& /*GameplayTageContainer*/, float /*DamageTaken*/);

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API URSHealthAttributeSet : public URSBaseAttributeSet
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URSHealthAttributeSet, Health);

	UPROPERTY(EditAnywhere, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URSHealthAttributeSet, MaxHealth);

	UPROPERTY(EditAnywhere, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageIn;
	ATTRIBUTE_ACCESSORS(URSHealthAttributeSet, DamageIn);


	mutable FDamageTakenEvent OnDamageTaken;
	//might want to add things like wing damage or things like that lter but this is a fine base
};

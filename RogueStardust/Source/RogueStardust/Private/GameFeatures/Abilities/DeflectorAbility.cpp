// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Abilities/DeflectorAbility.h"

/*virtual*/ void UDeflectorAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//This will activate the deflector but figure out how to do this with GAS
}

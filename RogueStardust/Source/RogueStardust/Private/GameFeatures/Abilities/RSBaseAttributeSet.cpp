// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/Abilities/RSBaseAttributeSet.h"

/*virtual*/ void URSBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	ClampAttributeChange(Attribute,NewValue);
}

/*virtual*/ void URSBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	ClampAttributeChange(Attribute, NewValue);
}

/*virtual*/ void URSBaseAttributeSet::ClampAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) const
{

}
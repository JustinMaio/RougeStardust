// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CharacterManager.generated.h"

UENUM()
enum ERSCharacterTypes
{
	EPlayer = 0,
	EBoss
};

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API UCharacterManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void RegisterCharacter(ERSCharacterTypes characterType, AActor* actor);

	TMap<ERSCharacterTypes, TObjectPtr<AActor>> m_CharacterMap;
};

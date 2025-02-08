// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CharacterManager.h"

void UCharacterManager::RegisterCharacter(ERSCharacterTypes characterType, AActor* actor)
{
	m_CharacterMap.FindOrAdd(characterType, actor);
}
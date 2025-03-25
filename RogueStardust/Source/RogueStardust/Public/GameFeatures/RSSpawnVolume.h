// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "DataHelpers/SpawnData.h"
#include "RSSpawnVolume.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESTARDUST_API ARSSpawnVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	ARSSpawnVolume();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* otherActor);

	UPROPERTY(EditAnywhere)
	TArray<FSpawnData> SpawnData;

private:
	bool m_HasTriggered = false;
};

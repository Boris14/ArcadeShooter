// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType) 
struct FWaveStruct : public FTableRowBase
{
	GENERATED_BODY()

	FWaveStruct() {
		SpaceDartCount = 0;
		SmartSpaceDartCount = 0;
		SpaceArcherCount = 0;
		SpaceTruckCount = 0;
		PlanetDestroyerCount = 0;
		Time = 0;
	}

	UPROPERTY(EditDefaultsOnly)
	int SpaceDartCount;

	UPROPERTY(EditDefaultsOnly)
	int SmartSpaceDartCount;

	UPROPERTY(EditDefaultsOnly)
	int SpaceArcherCount;

	UPROPERTY(EditDefaultsOnly)
	int SpaceTruckCount;

	UPROPERTY(EditDefaultsOnly)
	int PlanetDestroyerCount;

	UPROPERTY(EditDefaultsOnly)
	int Time;
};

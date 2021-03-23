// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponStruct() {
		RapidFireRate = 0;
		RadialProjectileDamage = 0;
		FrostSlowing = 0;
	}

	UPROPERTY(EditDefaultsOnly)
	float RapidFireRate;

	UPROPERTY(EditDefaultsOnly)
	float RadialProjectileDamage;

	UPROPERTY(EditDefaultsOnly)
	float FrostSlowing;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponStruct.generated.h"

/**
 * 
 */

USTRUCT()
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float RapidFireRate;

	UPROPERTY(EditDefaultsOnly)
	float RadialProjectileDamage;

	UPROPERTY(EditDefaultsOnly)
	float FrostSlowing;
};
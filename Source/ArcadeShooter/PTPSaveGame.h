// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PTPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API UPTPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, Category = Basic)
	uint32 UserIndex;

	UPTPSaveGame();
};

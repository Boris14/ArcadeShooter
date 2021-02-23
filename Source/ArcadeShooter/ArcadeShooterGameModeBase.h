// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"
#include "Planet.h"
#include "EnemySpawner.h"
#include "TimerManager.h"
#include "ArcadeShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ARCADESHOOTER_API AArcadeShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArcadeShooterGameModeBase();

	UFUNCTION(BlueprintCallable)
	void IncrementScore(float Delta);

	UFUNCTION(BlueprintCallable)
	FString GetWaveText();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DestroyGame();

	virtual void StartPlay() override;

	void FinishDisplayingWave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Score = 0;

	FTimerHandle MemberTimerHandle;

	AEnemySpawner* EnemySpawner;

	int PreviousWaveCount = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGameHasEnded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDeathScreenOn = false;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEnemySpawner> EnemySpawnerClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> PlayerClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlanet> PlanetClass;
};

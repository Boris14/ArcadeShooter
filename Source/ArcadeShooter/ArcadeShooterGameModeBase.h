// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"
#include "Planet.h"
#include "PlayerShipProjection.h"
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
	void IncrementScore(int Delta);

	UFUNCTION(BlueprintCallable)
	void IncrementGalaxyPoints(int Delta);

	UFUNCTION(BlueprintCallable)
	FString GetWaveText();

	UFUNCTION(BlueprintCallable)
	void StartLevel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndLevel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowDeathScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowLevelFinishedScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowUpgradeShip();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PurchaseNewPlayerShip();

	UFUNCTION(BlueprintCallable)
	bool UpgradeShip(FVector &MessageLocation);

	virtual void StartPlay() override;

	void FinishDisplayingWave();

	void SpawnPlayerProjection();

	bool CanSpawnPlayerShip();

	UFUNCTION(BlueprintCallable)
	void DestroyPlayerShipProjection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerShipProjection* PlayerShipProjection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Score = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int GalaxyPoints = 0;

	FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AEnemySpawner* EnemySpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AShip*> PlayerShips;

	int PreviousWaveCount = -1;

	int TotalLevels = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLevelHasEnded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDeathScreenOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLevelFinishedScreenOn = false;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEnemySpawner> EnemySpawnerClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> PlayerClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlanet> PlanetClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlayerShipProjection> PlayerProjectionClass;
};

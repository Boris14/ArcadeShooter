// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"
#include "Drop.h"
#include "Indicator.h"
#include "Planet.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PlayerShipProjection.h"
#include "EnemySpawner.h"
#include "Components/Widget.h"
#include "PopUpMessage.h"
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

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void IncrementScore(int Delta);

	UFUNCTION(BlueprintCallable)
	void IncrementGalaxyPoints(int Delta);

	UFUNCTION(BlueprintCallable)
	FString GetWaveText();

	AShip* SpawnNewPlayerShip(int CurrentPlayerShipsCount);

	APlayerShipProjection* SpawnPlayerShipProjection();

	void SpawnUpgradePopUp(FVector SpawnLocation);

	void SpawnNewShipPopUp();

	UFUNCTION(BlueprintCallable)
	void StartLevel();

	UFUNCTION(BlueprintCallable)
	void EndLevel();

	UFUNCTION(BlueprintCallable)
	void CalculateScore();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowDeathScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowLevelFinishedScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideDeathScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideLevelFinishedScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowUpgradeShip();

	void NotifyEnemySpawner(int PlayerShipsCount);

	virtual void StartPlay() override;

	void FinishDisplayingWave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Score = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int GalaxyPoints = 0;

	FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AEnemySpawner* EnemySpawner;

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
	TSubclassOf<AEnemySpawner> EnemySpawnerClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AShip> PlayerClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlanet> PlanetClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerShipProjection> PlayerProjectionClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APopUpMessage> PopUpMessageClass;
};

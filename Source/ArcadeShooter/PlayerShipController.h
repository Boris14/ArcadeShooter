// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"
#include "PopUpMessage.h"
#include "EnemySpawner.h"
#include "PlayerShipProjection.h"
#include "ArcadeShooterGameModeBase.h"
#include "TimerManager.h"
#include "PlayerShipController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API APlayerShipController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerShipController();

	virtual void SetupInputComponent() override;

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

	virtual void BeginInactiveState();

	virtual void OnPossess(APawn* InPawn);

	virtual void OnUnPossess();

	void MovePlayerShips(float AxisValue, float DeltaTime);
	
	void PurchaseUpgrade();

	void PurchaseNewShip();

	void Fire(float AxisValue);

	void RestoreNormalSpeed();

	void StartLevel();

	void NotifyEnemySpawner();

	FTimerHandle MemberTimerHandle;

	AArcadeShooterGameModeBase* GameMode;

	bool bIsWaitingForPlayerShip = false;

	TArray<AShip*> PlayerShips;

	APlayerShipProjection* PlayerShipProjection;
};

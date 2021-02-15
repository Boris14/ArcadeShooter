// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Ship.h"
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

	void MovePlayerShip(float AxisValue);

	void ChangeWeapon();

	void Fire(float AxisValue);

	void RestartGame();

	void Reload();

	FTimerHandle MemberTimerHandle;

	bool bCanShoot = true;
};

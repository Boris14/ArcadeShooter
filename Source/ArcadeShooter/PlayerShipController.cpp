// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipController.h"


APlayerShipController::APlayerShipController() {
}

void APlayerShipController::SetupInputComponent() {
	
	Super::SetupInputComponent();

	InputComponent->BindAction("PurchaseUpgrade", IE_Pressed, this, &APlayerShipController::PurchaseUpgrade);

	InputComponent->BindAction("PruchaseNewShip", IE_Pressed, this, &APlayerShipController::PurchaseNewShip);

	InputComponent->BindAction("StartLevel", IE_Pressed, this, &APlayerShipController::StartLevel);

	InputComponent->BindAxis("MoveClockwise", this, &APlayerShipController::MovePlayerShip);

	InputComponent->BindAxis("Fire", this, &APlayerShipController::Fire);
}

void APlayerShipController::MovePlayerShip(float AxisValue) {

	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
		for (AShip* Ship : GameMode->PlayerShips) {
			Ship->CalculateMovement(AxisValue);
		}
	}
}

void APlayerShipController::Fire(float AxisValue)
{
	if (AxisValue == 1) {

		AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
			for (AShip* Ship : GameMode->PlayerShips) {
				if (Ship->bCanShoot) {
					Ship->SetShootingSpeed();
					Ship->Fire();
					if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
						GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerShipController::RestoreNormalSpeed, 0.1, true, 0.1);
					}
				}
			}
		}	
	}
}

void APlayerShipController::RestoreNormalSpeed()
{
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
		for (AShip* Ship : GameMode->PlayerShips) {
			if (!Ship->bCanShoot) {
				return;
			}
		}
		for (AShip* Ship : GameMode->PlayerShips) {
			Ship->SetNormalSpeed();
		}
	}
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}

void APlayerShipController::PurchaseUpgrade() 
{
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (IsValid(GameMode)) {
		if (GameMode->GalaxyPoints >= 600 && !GameMode->bLevelHasEnded) {
			GameMode->UpgradePlayerShip();
		}
	}

}

void APlayerShipController::StartLevel()
{
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode)) {
		if (GameMode->bLevelHasEnded && GameMode->Level < GameMode->TotalLevels) {
			GameMode->StartLevel();
		}
	}
}

void APlayerShipController::PurchaseNewShip()
{
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode)) {
		if (GameMode->GalaxyPoints >= 200 && !GameMode->bLevelHasEnded) {
			GameMode->PurchaseNewPlayerShip();
		}
	}
}
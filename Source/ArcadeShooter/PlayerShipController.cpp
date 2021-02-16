// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipController.h"


APlayerShipController::APlayerShipController() {
}

void APlayerShipController::SetupInputComponent() {
	
	Super::SetupInputComponent();

	InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &APlayerShipController::ChangeWeapon);

	InputComponent->BindAction("RestartGame", IE_Pressed, this, &APlayerShipController::RestartGame);

	InputComponent->BindAxis("MoveClockwise", this, &APlayerShipController::MovePlayerShip);

	InputComponent->BindAxis("Fire", this, &APlayerShipController::Fire);
}

void APlayerShipController::MovePlayerShip(float AxisValue) {

	AShip* PlayerShip = Cast<AShip>(GetPawn());

	if (PlayerShip != nullptr) {
		PlayerShip->CalculateMovement(AxisValue);
	}
}

void APlayerShipController::Fire(float AxisValue)
{
	if (AxisValue == 1) {
		AShip* PlayerShip = Cast<AShip>(GetPawn());

		if (PlayerShip != nullptr) {
			PlayerShip->SetShootingSpeed();
		}

		if (bCanShoot) {
			if (PlayerShip != nullptr) {
				PlayerShip->Fire();
				bCanShoot = false;
				GetWorldTimerManager().SetTimer(MemberTimerHandle, 
												this, 
												&APlayerShipController::Reload, 
												PlayerShip->GetFireRate(), 
												false, 
												PlayerShip->GetFireRate());
			}
		}
	}
}

void APlayerShipController::Reload()
{
	bCanShoot = true;

	AShip* PlayerShip = Cast<AShip>(GetPawn());

	if (PlayerShip != nullptr) {
		PlayerShip->SetNormalSpeed();
	}
}

void APlayerShipController::ChangeWeapon() 
{
	AShip* PlayerShip = Cast<AShip>(GetPawn());

	if (PlayerShip != nullptr) {
		PlayerShip->ChangeWeapon();
	}

}

void APlayerShipController::RestartGame()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArcadeShooterGameModeBase::StaticClass(), FoundActors);
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(FoundActors[0]);
	if (GameMode != nullptr) {
		if (GameMode->bGameHasEnded) {
			GameMode->StartGame();
		}
	}
}
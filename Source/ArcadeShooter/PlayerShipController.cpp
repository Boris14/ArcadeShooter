// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipController.h"


APlayerShipController::APlayerShipController() {
}

void APlayerShipController::SetupInputComponent() {
	
	Super::SetupInputComponent();

	InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &APlayerShipController::PurchaseUpgrade);

	InputComponent->BindAction("StartGame", IE_Pressed, this, &APlayerShipController::StartGame);

	InputComponent->BindAxis("MoveClockwise", this, &APlayerShipController::MovePlayerShip);

	InputComponent->BindAxis("Fire", this, &APlayerShipController::Fire);
}

void APlayerShipController::MovePlayerShip(float AxisValue) {

	AShip* PlayerShip = Cast<AShip>(GetPawn());

	if (IsValid(PlayerShip)) {
		PlayerShip->CalculateMovement(AxisValue);
	}
}

void APlayerShipController::Fire(float AxisValue)
{
	if (AxisValue == 1) {
		AShip* PlayerShip = Cast<AShip>(GetPawn());

		if (IsValid(PlayerShip)) {
			PlayerShip->SetShootingSpeed();
		}

		if (bCanShoot) {
			if (IsValid(PlayerShip)) {
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

	if (IsValid(PlayerShip)) {
		PlayerShip->SetNormalSpeed();
	}
}

void APlayerShipController::PurchaseUpgrade() 
{
	AShip* PlayerShip = Cast<AShip>(GetPawn());

	if (IsValid(PlayerShip)) {
		PlayerShip->PurchaseUpgrade();
	}

}

void APlayerShipController::StartGame()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArcadeShooterGameModeBase::StaticClass(), FoundActors);
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(FoundActors[0]);
	if (IsValid(GameMode)) {
		if (GameMode->bGameHasEnded && GameMode->Level < GameMode->TotalLevels) {
			GameMode->StartGame();
		}
	}
}
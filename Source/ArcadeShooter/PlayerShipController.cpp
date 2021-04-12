// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipController.h"

void APlayerShipController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void APlayerShipController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlayerShips(GetInputAxisValue("MoveClockwise"), DeltaTime);
	
	if (PlayerShipProjection != nullptr && !IsValid(PlayerShipProjection)) {
		PlayerShipProjection = nullptr;
	}

	for (int i = 0; i < PlayerShips.Num(); ++i) {
		if (!IsValid(PlayerShips[i])) {
			PlayerShips.RemoveAt(i);
			NotifyGameMode();
		}
	}

	if (IsValid(GameMode) && !GameMode->bLevelHasEnded &&
		PlayerShips.Num() < GameMode->PlayerShipsCount) {
		PlayerShips.Push(GameMode->SpawnNewPlayerShip(PlayerShips.Num()));
		Possess(PlayerShips[0]);
	}
}

void APlayerShipController::NotifyGameMode()
{
	if (IsValid(GameMode)) {
		GameMode->NotifyEnemySpawner(PlayerShips.Num());
	}
}

void APlayerShipController::SetupInputComponent() {
	
	Super::SetupInputComponent();

	InputComponent->BindAction("PurchaseUpgrade", IE_Pressed, this, &APlayerShipController::PurchaseUpgrade);

	InputComponent->BindAction("PruchaseNewShip", IE_Pressed, this, &APlayerShipController::PurchaseNewShip);

	InputComponent->BindAction("StartLevel", IE_Pressed, this, &APlayerShipController::StartLevel);

	InputComponent->BindAxis("MoveClockwise");

	InputComponent->BindAxis("Fire", this, &APlayerShipController::Fire);
}

void APlayerShipController::BeginInactiveState()
{
	Super::BeginInactiveState();

	if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
		if (PlayerShips.Num() > 0 && IsValid(PlayerShips[0])) {
			Possess(PlayerShips[0]);
		}
	}
}

void APlayerShipController::MovePlayerShips(float AxisValue, float DeltaTime) 
{
	if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
		for (AShip* Ship : PlayerShips) {
			if (IsValid(Ship)) {
				Ship->CalculateMovement(AxisValue, DeltaTime);
			}
		}
	}
}

void APlayerShipController::Fire(float AxisValue)
{
	if (AxisValue == 1) {
		if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
			for (AShip* Ship : PlayerShips) {
				if (IsValid(Ship)) {
					if (Ship->bCanShoot) {	
						Ship->Fire();
						CalculateVolumeMultiplier(Ship);
						GameMode->PlayPlayerFiringSound(Ship->GetWeaponType(), Ship->GetActorLocation(), Ship->VolumeMultiplier);
						if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
							GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerShipController::RestoreNormalSpeed, 0.1, true, 0.1);
						}
					}
				}
			}
		}	
	}
}

void APlayerShipController::RestoreNormalSpeed()
{
	if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
		int i = 0;
		for (AShip* Ship : PlayerShips) {
			if (IsValid(Ship)) {
				if (!Ship->bCanShoot) {
					return;
				}
			}
			i++;
		}
		for (AShip* Ship : PlayerShips) {
			if (IsValid(Ship)) {
				Ship->SetNormalSpeed();
			}
		}
	}
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}

void APlayerShipController::PurchaseUpgrade() 
{
	if (IsValid(GameMode)) {
		if (!GameMode->bLevelHasEnded) {
			if (GameMode->GalaxyPoints >= 400) {
				for (AShip* Ship : PlayerShips) {
					if (IsValid(Ship)) {
						if (Ship->Upgrade()) {
							GameMode->GalaxyPoints -= 400;
							GameMode->ShowShipMessage(Ship->GetActorLocation(), Ship->Level + 1);
							break;
						}
					}
				}
			}
			else {
				bShowNotEnoughGP = true;
				GetWorldTimerManager().SetTimer(MessageTimerHandle, this, 
					&APlayerShipController::TurnOffShowNotEnoughGP, 1.5, false, 1.5);
			}
		}
	}
}

void APlayerShipController::StartLevel()
{
	if (IsValid(GameMode)) {
		if (GameMode->bLevelHasEnded) {
			PlayerShips.Empty();
			NotifyGameMode();
			if (GameMode->Level < GameMode->TotalLevels) {
				GameMode->StartLevel();
			}
			else {
				GameMode->ReturnToMainMenu();
			}
		}
	}
}

void APlayerShipController::PurchaseNewShip()
{
	if (IsValid(GameMode)) {
		if (!GameMode->bLevelHasEnded) {
			if (GameMode->GalaxyPoints >= 400) {
				if (IsValid(PlayerShipProjection)) {
					if (!PlayerShipProjection->bIsOverlapping) {
						GameMode->GalaxyPoints -= 400;
						PlayerShipProjection->Destroy();
						AShip* NewPlayerShip = GameMode->SpawnNewPlayerShip(PlayerShips.Num());
						GameMode->ShowShipMessage(NewPlayerShip->GetActorLocation(), NewPlayerShip->Level + 1);
						PlayerShips.Push(NewPlayerShip);
					}
				}
				else {
					PlayerShipProjection = GameMode->SpawnPlayerShipProjection();
				}
			}
			else {
				bShowNotEnoughGP = true;
				GetWorldTimerManager().SetTimer(MessageTimerHandle, this,
					&APlayerShipController::TurnOffShowNotEnoughGP, 1.5, false, 1.5);
			}
		}
	}
}

void APlayerShipController::CalculateVolumeMultiplier(AShip* GivenShip)
{
	if (IsValid(GameMode)) {
		float SameShipsCount = 0;

		for (AShip* Ship : PlayerShips) {
			if (IsValid(Ship)) {
				if (Ship->GetWeaponType() == GivenShip->GetWeaponType()) {
					if (Ship->GetWeaponType() != WeaponType::Rapid || 
						Ship->Level == GivenShip->Level) {
						SameShipsCount++;
					}
					else if (Ship->Level > GivenShip->Level) {
						GivenShip->VolumeMultiplier = 0;
						return;
					}
				}
			}
		}

		GivenShip->VolumeMultiplier = 1 / SameShipsCount;
	}
	else {
		GivenShip->VolumeMultiplier = 1;
	}
}

void APlayerShipController::TurnOffShowNotEnoughGP()
{
	bShowNotEnoughGP = false;
}
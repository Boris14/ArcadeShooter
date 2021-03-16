// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipController.h"


APlayerShipController::APlayerShipController() 
{
}

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
			NotifyEnemySpawner();
		}
	}

	if (bIsWaitingForPlayerShip) {
		if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
			PlayerShips.Push(GameMode->SpawnNewPlayerShip(PlayerShips.Num()));
			Possess(PlayerShips[0]);
			bIsWaitingForPlayerShip = false;
		}
	}
}

void APlayerShipController::NotifyEnemySpawner()
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
		if (PlayerShips.Num() < 1) {
			PlayerShips.Push(GameMode->SpawnNewPlayerShip(PlayerShips.Num()));
		}
		if (IsValid(PlayerShips[0])) {
			Possess(PlayerShips[0]);
		}
		else {
			PlayerShips.Empty();
			NotifyEnemySpawner();
		}
	}
	else {
		PlayerShips.Empty();
		NotifyEnemySpawner();
	}
}

void APlayerShipController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void APlayerShipController::OnUnPossess()
{
	Super::OnUnPossess();
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
}

void APlayerShipController::RestoreNormalSpeed()
{
	if (IsValid(GameMode) && !GameMode->bLevelHasEnded) {
		for (AShip* Ship : PlayerShips) {
			if (IsValid(Ship)) {
				if (!Ship->bCanShoot) {
					return;
				}
			}
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
		if (GameMode->GalaxyPoints >= 400 && !GameMode->bLevelHasEnded) {
			for (AShip* Ship : PlayerShips) {
				if (IsValid(Ship)) {
					if (Ship->Upgrade()) {
						GameMode->GalaxyPoints -= 400;
						GameMode->SpawnUpgradePopUp(Ship->GetActorLocation());
						break;
					}
				}
			}
		}
	}

}

void APlayerShipController::StartLevel()
{
	if (IsValid(GameMode)) {
		if (GameMode->bLevelHasEnded && GameMode->Level < GameMode->TotalLevels) {
			GameMode->StartLevel();
			PlayerShips.Empty();
			NotifyEnemySpawner();
			bIsWaitingForPlayerShip = true;
		}
	}
}

void APlayerShipController::PurchaseNewShip()
{
	if (IsValid(GameMode)) {
		if (!GameMode->bLevelHasEnded && GameMode->GalaxyPoints >= 400 && IsValid(PlayerShipProjection)) {
			if (!PlayerShipProjection->bIsOverlapping) {
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bought a new ship")));
				GameMode->GalaxyPoints -= 400;
				PlayerShipProjection->Destroy();
				PlayerShips.Push(GameMode->SpawnNewPlayerShip(PlayerShips.Num()));
				GameMode->SpawnNewShipPopUp();
			}
		}
		else if (!GameMode->bLevelHasEnded && GameMode->GalaxyPoints >= 400) {
			PlayerShipProjection = GameMode->SpawnPlayerShipProjection();
		}
	}
}
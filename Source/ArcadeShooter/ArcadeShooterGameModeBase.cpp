// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcadeShooterGameModeBase.h"


AArcadeShooterGameModeBase::AArcadeShooterGameModeBase()
{
	DefaultPawnClass = AShip::StaticClass();
}

void AArcadeShooterGameModeBase::IncrementScore(int Delta)
{
	Score += Delta;
}

void AArcadeShooterGameModeBase::IncrementGalaxyPoints(int Delta)
{
	GalaxyPoints += Delta;
}

FString AArcadeShooterGameModeBase::GetWaveText()
{
	FString Result = "";

	if (IsValid(EnemySpawner)) {

		if (PreviousWaveCount == EnemySpawner->GetCurrWaveCount()) {
			if (EnemySpawner->bLevelFinished) {
				EndLevel();
				ShowLevelFinishedScreen();
			}
			return Result;
		}
		else {
			if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
				GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
					&AArcadeShooterGameModeBase::FinishDisplayingWave, 7.0f, false, 7.0f);
			}
		}

		Result = "Wave " + FString::FromInt(EnemySpawner->GetCurrWaveCount() + 1);
	}

	return Result;
}

void AArcadeShooterGameModeBase::StartLevel()
{
	EndLevel();
	PlayerShips.Empty();
	bLevelHasEnded = false;

	EnemySpawner = Cast<AEnemySpawner>(GetWorld()->SpawnActor(EnemySpawnerClass));
	EnemySpawner->PlayerShipsCount = 1;

	APlanet* Planet = GetWorld()->SpawnActor<APlanet>(PlanetClass, 
									FVector(0,0,0), 
									FRotator(0,0,0));

	PlayerShips.Add(GetWorld()->SpawnActor<AShip>(PlayerClass, 
														FVector(0, Planet->Diameter, 0), 
														FRotator(0, 0, 0)));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->Possess(PlayerShips[0]);
}

void AArcadeShooterGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (Level < TotalLevels) {
		StartLevel();
	}
}

void AArcadeShooterGameModeBase::FinishDisplayingWave()
{
	if (IsValid(EnemySpawner)) {
		PreviousWaveCount = EnemySpawner->GetCurrWaveCount();
	}
}

void AArcadeShooterGameModeBase::SpawnPlayerProjection()
{
	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
	APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);
	if (IsValid(Planet)) {
		PlayerShipProjection = Cast<APlayerShipProjection>(
			GetWorld()->SpawnActor<APlayerShipProjection>(PlayerProjectionClass,
				FVector(0, Planet->Diameter, 0),
				FRotator(0, 90, 0))
			);
	}
}

bool AArcadeShooterGameModeBase::CanSpawnPlayerShip()
{
	if (IsValid(PlayerShipProjection)) {
		return !PlayerShipProjection->bIsOverlapping;
	}
	return false;
}

void AArcadeShooterGameModeBase::DestroyPlayerShipProjection()
{
	PlayerShipProjection->Destroy();
	PlayerShipProjection = nullptr;
}

bool AArcadeShooterGameModeBase::UpgradeShip(FVector& MessageLocation)
{
	for (AShip* Ship : PlayerShips) {
		if (IsValid(Ship)) {
			if (Ship->Upgrade()) {
				MessageLocation = Ship->GetActorLocation();
				return true;
			}
		}
	}
	return false;
}
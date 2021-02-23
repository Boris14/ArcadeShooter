// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcadeShooterGameModeBase.h"


AArcadeShooterGameModeBase::AArcadeShooterGameModeBase()
{
	DefaultPawnClass = AShip::StaticClass();
}

void AArcadeShooterGameModeBase::IncrementScore(float Delta)
{
	Score = Score + Delta;
}

FString AArcadeShooterGameModeBase::GetWaveText()
{
	FString Result = "";

	if (IsValid(EnemySpawner)) {

		if (PreviousWaveCount == EnemySpawner->GetCurrWaveCount()) {
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

void AArcadeShooterGameModeBase::StartGame()
{
	EnemySpawner = Cast<AEnemySpawner>(GetWorld()->SpawnActor(EnemySpawnerClass));
	GetWorld()->SpawnActor<APlanet>(PlanetClass, 
									FVector(0,0,0), 
									FRotator(0,0,0));

	AShip* PlayerShip = GetWorld()->SpawnActor<AShip>(PlayerClass, 
														FVector(800, 0, 0), 
														FRotator(0, 0, 0));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->Possess(PlayerShip);

	bGameHasEnded = false;
	Score = 0;
}

void AArcadeShooterGameModeBase::StartPlay()
{
	Super::StartPlay();

	StartGame();
}

void AArcadeShooterGameModeBase::FinishDisplayingWave()
{
	if (IsValid(EnemySpawner)) {
		PreviousWaveCount = EnemySpawner->GetCurrWaveCount();
	}
}
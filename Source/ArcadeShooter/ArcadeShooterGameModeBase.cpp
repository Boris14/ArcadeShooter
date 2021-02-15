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

void AArcadeShooterGameModeBase::StartGame()
{
	GetWorld()->SpawnActor(EnemySpawnerClass);
	GetWorld()->SpawnActor<APlanet>(PlanetClass, FVector(0,0,0), FRotator(0,0,0));

	AShip* PlayerShip = GetWorld()->SpawnActor<AShip>(PlayerClass, FVector(800, 0, 0), FRotator(0, 0, 0));
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
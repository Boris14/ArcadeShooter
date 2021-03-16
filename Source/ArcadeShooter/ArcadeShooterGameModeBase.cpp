// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcadeShooterGameModeBase.h"


AArcadeShooterGameModeBase::AArcadeShooterGameModeBase()
{
	DefaultPawnClass = AShip::StaticClass();
}

void AArcadeShooterGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bLevelHasEnded) {
		if (bDeathScreenOn) {
			HideDeathScreen();
		}
		else if(bLevelFinishedScreenOn){
			HideLevelFinishedScreen();
		}
	}
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
				Level++;
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

void AArcadeShooterGameModeBase::CalculateScore()
{
	if (GalaxyPoints > 0) {
		Score = Score + (GalaxyPoints * 10);
		GalaxyPoints = 0;
	}
}

AShip* AArcadeShooterGameModeBase::SpawnNewPlayerShip(int CurrentShipsCount)
{
	NotifyEnemySpawner(CurrentShipsCount + 1);

	return GetWorld()->SpawnActor<AShip>(PlayerClass, 
		FVector(0, 630, 0),
		FRotator(0, 0, 0));
}

APlayerShipProjection* AArcadeShooterGameModeBase::SpawnPlayerShipProjection()
{
	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
	APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);

	return GetWorld()->SpawnActor<APlayerShipProjection>(PlayerProjectionClass,
		FVector(0, Planet->Radius, 0),
		FRotator(0, 90, 0)
		);
}

void AArcadeShooterGameModeBase::SpawnUpgradePopUp(FVector SpawnLocation)
{
	APopUpMessage* Message = GetWorld()->SpawnActor<APopUpMessage>(PopUpMessageClass,
		SpawnLocation,
		FRotator(180, 0, 180));
	if (IsValid(Message)) {
		Message->SetTexts("Upgrade", "-400");
		Message->SetColor(true, Message->ScoreColor);
		Message->SetColor(false, Message->GPColor);
	}
}

void AArcadeShooterGameModeBase::SpawnNewShipPopUp()
{
	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
	APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);

	APopUpMessage* Message = GetWorld()->SpawnActor<APopUpMessage>(PopUpMessageClass,
		FVector(0, Planet->Radius, 0),
		FRotator(180, 0, 180));
	if (IsValid(Message)) {
		Message->SetTexts("NewShip", "-400");
		Message->SetColor(true, Message->ScoreColor);
		Message->SetColor(false, Message->GPColor);
	}
}

void AArcadeShooterGameModeBase::StartLevel()
{
	EndLevel();
	bLevelHasEnded = false;

	EnemySpawner = Cast<AEnemySpawner>(GetWorld()->SpawnActor(EnemySpawnerClass));
	EnemySpawner->PlayerShipsCount = 1;

	APlanet* Planet = GetWorld()->SpawnActor<APlanet>(PlanetClass, 
									FVector(0,0,0), 
									FRotator(0,0,0));
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

void AArcadeShooterGameModeBase::EndLevel()
{
	CalculateScore();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), 
										AActor::StaticClass(), 
										FoundActors);

	for (AActor* Actor : FoundActors) {
		if (IsValid(Cast<AShip>(Actor)) || 
			IsValid(Cast<AProjectile>(Actor)) ||
			IsValid(Cast<AEnemySpawner>(Actor)) || 
			IsValid(Cast<APlanet>(Actor)) ||
			IsValid(Cast<AShip>(Actor)) || 
			IsValid(Cast<ADrop>(Actor)) ||
			IsValid(Cast<AIndicator>(Actor)) || 
			IsValid(Cast<APlayerShipProjection>(Actor))) 
		{
			Actor->Destroy();
		}
	}

	bLevelHasEnded = true;
}

void AArcadeShooterGameModeBase::NotifyEnemySpawner(int NewPlayerShipsCount)
{
	if (IsValid(EnemySpawner)) {
		EnemySpawner->PlayerShipsCount = NewPlayerShipsCount;
	}
}
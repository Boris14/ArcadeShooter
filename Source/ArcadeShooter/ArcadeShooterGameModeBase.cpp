// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcadeShooterGameModeBase.h"


AArcadeShooterGameModeBase::AArcadeShooterGameModeBase()
{
	DefaultPawnClass = AShip::StaticClass();
}

void AArcadeShooterGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(EnemySpawner) && EnemySpawner != nullptr) {
		EnemySpawner = nullptr;
	}

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
	CurrLevelScore += Delta;
}

void AArcadeShooterGameModeBase::IncrementGalaxyPoints(int Delta)
{
	GalaxyPoints += Delta;
}

FString AArcadeShooterGameModeBase::ChangeWaveAndGetText()
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

		if (Level != TotalLevels - 1) {
			Result = "Wave " + FString::FromInt(EnemySpawner->GetCurrWaveCount() + 1);
		}
		else {
			Result = "Boss";
		}
	}

	return Result;
}

void AArcadeShooterGameModeBase::CalculateScore(int PlanetHealth)
{
	if (!bShouldResetScore) {
		TotalScore += CurrLevelScore;
	
		TotalScore += PlanetHealth * 600;

		if (GalaxyPoints > 0) {
			TotalScore += GalaxyPoints * 10;
			GalaxyPoints = 0;
		}
		if (PlayerShipsCount > 0) {
			TotalScore += PlayerShipsCount * 500;
			PlayerShipsCount = 0;
		}
	}
}

AShip* AArcadeShooterGameModeBase::SpawnNewPlayerShip(int CurrentShipsCount)
{
	NotifyEnemySpawner(CurrentShipsCount + 1);

	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
	if (FoundPlanets.Num() > 0) {
		APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);
		if (IsValid(Planet)) {
			return GetWorld()->SpawnActor<AShip>(PlayerClass,
				FVector(0, Planet->Radius, 0),
				FRotator(0, 0, 0));
		}
	}

	return nullptr;
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

void AArcadeShooterGameModeBase::ShowUpgrade(FVector Location)
{
	APopUpMessage* Message = GetWorld()->SpawnActor<APopUpMessage>(PopUpMessageClass,
		Location,
		FRotator(180, 0, 180));
	if (IsValid(Message)) {
		Message->SetTexts("Upgrade", "-400");
		Message->SetColor(true, Message->ScoreColor);
		Message->SetColor(false, Message->GPColor);
	}
	PlayUpgradeSound(Location);
}

void AArcadeShooterGameModeBase::ShowNewShip()
{
	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);

	if (FoundPlanets.Num() > 0) {
		APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);

		FVector SpawnLocation = FVector(0, Planet->Radius, 0);

		APopUpMessage* Message = GetWorld()->SpawnActor<APopUpMessage>(PopUpMessageClass,
			SpawnLocation,
			FRotator(180, 0, 180));
		if (IsValid(Message)) {
			Message->SetTexts("New Ship", "-400");
			Message->SetColor(true, Message->ScoreColor);
			Message->SetColor(false, Message->GPColor);
		}
		PlayNewShipSound(SpawnLocation);
	}
}

void AArcadeShooterGameModeBase::StartLevel()
{
	CurrLevelScore = 0;
	GalaxyPoints = 0;
	EndLevel();
	bLevelHasEnded = false;
	bShouldResetScore = false;

	EnemySpawner = Cast<AEnemySpawner>(GetWorld()->SpawnActor(EnemySpawnerClass));
	NotifyEnemySpawner(1);

	APlanet* Planet = GetWorld()->SpawnActor<APlanet>(PlanetClass, 
									FVector(0,0,0), 
									FRotator(0,0,0));

	PlayBackgroundMusic();
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
	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
	if (FoundPlanets.Num() > 0) {
		APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);

		if (IsValid(Planet)) {
			CalculateScore(Planet->Health);
		}
	}
	else {
		CalculateScore(0);
	}

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

	StopPlayingBackgroundMusic();
	PlayerShipsCount = 0;
	bLevelHasEnded = true;
}

void AArcadeShooterGameModeBase::NotifyEnemySpawner(int NewPlayerShipsCount)
{
	PlayerShipsCount = NewPlayerShipsCount;
	if (IsValid(EnemySpawner)) {
		EnemySpawner->PlayerShipsCount = PlayerShipsCount;
	}
}

void AArcadeShooterGameModeBase::ResetScore()
{
	bShouldResetScore = true;
}

void AArcadeShooterGameModeBase::PlayPlayerFiringSound(WeaponType Weapon, FVector Location, float VolumeMult)
{
	switch (Weapon) {
	case WeaponType::Rapid:
			PlayRapidFireSound(Location, VolumeMult);
		break;
	case WeaponType::Radial:
			PlayRadialFireSound(Location, VolumeMult);
		break;
	case WeaponType::Frost:
			PlayFrostFireSound(Location, VolumeMult);
		break;
	default:
		break;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnDistance = 3500;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentLevel == 100 && !bHighscoreModeOn) {
		bHighscoreModeOn = true;
		StartHighscoreLevel();
	}
	else if(Levels.Num() < 1 && IsValid(Level1) && IsValid(Level2) &&
		IsValid(Level3) && IsValid(Level4) && IsValid(Level5) && CurrentLevel != 100) {
		Levels.Add(Level1);
		Levels.Add(Level2);
		Levels.Add(Level3);
		Levels.Add(Level4);
		Levels.Add(Level5);
		StartNormalLevel();
	}

}

void AEnemySpawner::StartNormalLevel()
{
	const FString Context(TEXT("Wave"));

	FString WaveRowIndex = "";

	for (int i = 1; ; ++i) {
		WaveRowIndex = "Wave" + FString::FromInt(i);
		FWaveStruct* WaveRow = Levels[CurrentLevel]->FindRow<FWaveStruct>(FName(WaveRowIndex), Context, true);

		if (WaveRow) {
			Waves.Add(WaveRow);
		}
		else {
			TotalWaves = i - 1;
			break;
		}
	}

	CurrWaveCount = 0;
	TransferWaveData(Waves[CurrWaveCount]);

	SetWaveTimer(Waves[CurrWaveCount]->Time);
}

void AEnemySpawner::StartHighscoreLevel()
{
	const FString Context(TEXT("Wave"));
	TransferWaveData(HighscoreLevel->FindRow<FWaveStruct>(FName("EndlessWave"), Context, true));
	WaveEnemiesLeft = 9999;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawner::SpawnEnemies, 10.0f, false, 10.0f);
}

void AEnemySpawner::SetWaveTimer(float WaveTime)
{
	WaveEnemiesLeft = Waves[CurrWaveCount]->SpaceDartCount +
		Waves[CurrWaveCount]->SmartSpaceDartCount +
		Waves[CurrWaveCount]->SpaceArcherCount +
		Waves[CurrWaveCount]->SpaceTruckCount + 
		Waves[CurrWaveCount]->PlanetDestroyerCount;
	
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawner::SpawnEnemies, WaveTime / WaveEnemiesLeft, true, 10.0f);
}

void AEnemySpawner::SpawnEnemies()
{
	AShip* SpawnedShip = nullptr;

	for (int i = 0; i < EnemiesToSpawnAtOnce; ++i) {
		float Angle = FMath::RandRange(0.0f, 360.0f);
		float X = sin(Angle * (PI / 180)) * SpawnDistance;
		float Y = cos(Angle * (PI / 180)) * SpawnDistance;

		FVector SpawnPoint = FVector(X, Y, 0);
		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint,
			FVector(0, 0, 0));
		SpawnRotation.Yaw = 270 - Angle;

		int32 GeneratedNum;
		bool bHasEnemy = false;

		while (!bHasEnemy && WaveEnemiesLeft > 0) {

			GeneratedNum = FMath::RandRange(0, 4);

			if (GeneratedNum == 0 && CurrentWave.SpaceDartCount > 0) {
				SpawnedShip = GetWorld()->SpawnActor<AShip>(SpaceDartClass, SpawnPoint, SpawnRotation);
				CurrentWave.SpaceDartCount--;
				bHasEnemy = true;
			}
			else if (GeneratedNum == 1 && CurrentWave.SmartSpaceDartCount > 0) {
				SpawnedShip = GetWorld()->SpawnActor<AShip>(SmartSpaceDartClass, SpawnPoint, SpawnRotation);
				CurrentWave.SmartSpaceDartCount--;
				bHasEnemy = true;
			}
			else if (GeneratedNum == 2 && CurrentWave.SpaceArcherCount > 0) {
				SpawnedShip = GetWorld()->SpawnActor<AShip>(SpaceArcherClass, SpawnPoint, SpawnRotation);
				CurrentWave.SpaceArcherCount--;
				bHasEnemy = true;
			}
			else if (GeneratedNum == 3 && CurrentWave.SpaceTruckCount > 0) {
				SpawnedShip = GetWorld()->SpawnActor<AShip>(SpaceTruckClass, SpawnPoint, SpawnRotation);
				CurrentWave.SpaceTruckCount--;
				bHasEnemy = true;
			}
			else if (CurrentWave.PlanetDestroyerCount > 0) {
				SpawnedShip = GetWorld()->SpawnActor<AShip>(PlanetDestroyerClass, SpawnPoint, SpawnRotation);
				CurrentWave.PlanetDestroyerCount--;
				bHasEnemy = true;
			}

			if (bHasEnemy) {
				WaveEnemiesLeft--;
			}
		}

		if (bHasEnemy && IsValid(SpawnedShip)) {
			SpawnedShip->Initialize(Angle);
		}
		else if (!bHighscoreModeOn) {
			GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
				&AEnemySpawner::CheckWaveFinished,
				1.0f, true, 1.0f);
			break;
		}
	}

	if (bHighscoreModeOn) {
		CurrentWave.Time -= SpawnTimeDeceleration;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
			&AEnemySpawner::SpawnEnemies,
			CurrentWave.Time, false, CurrentWave.Time);
		IncreaseHighscoreModeDifficulty();
	}
}

void AEnemySpawner::IncreaseHighscoreModeDifficulty()
{
	TArray<AActor*> FoundDestroyers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		APlanetDestroyer::StaticClass(),
		FoundDestroyers);

	if (CurrentWave.Time > SpawnTimeInterval * 2 &&
			FoundDestroyers.Num() == EnemiesToSpawnAtOnce) {
		EnemiesToSpawnAtOnce++;
		CurrentWave.Time = SpawnTimeInterval;
		SpawnTimeDeceleration *= 1.5;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
			&AEnemySpawner::CheckPlanetDestroyer,
			1, true, 1);

		return;
	}
	else if (CurrentWave.Time <= SpawnTimeInterval * 0.97 && 
			CurrentWave.Time > SpawnTimeInterval * 0.5 && 
			CurrentWave.SmartSpaceDartCount <= 0) {
		CurrentWave.SmartSpaceDartCount = 999;
	}
	else if (CurrentWave.Time <= SpawnTimeInterval * 0.9 && 
			CurrentWave.Time > SpawnTimeInterval * 0.5 && 
			CurrentWave.SpaceArcherCount <= 0) {
		CurrentWave.SpaceArcherCount = 999;
	}
	else if (CurrentWave.Time <= SpawnTimeInterval * 0.8 && 
			CurrentWave.Time > SpawnTimeInterval * 0.5 && 
			CurrentWave.SpaceTruckCount <= 0) {
		CurrentWave.SpaceTruckCount = 999;
	}
	else if (CurrentWave.Time <= SpawnTimeInterval * 0.7 && 
			CurrentWave.SpaceDartCount > 0) {
		CurrentWave.SpaceDartCount = 0;
	}
	else if (CurrentWave.Time <= SpawnTimeInterval * 0.6 &&
		CurrentWave.PlanetDestroyerCount <= 0) {

		CurrentWave.SmartSpaceDartCount = 0;
		CurrentWave.SpaceArcherCount = 0;
		CurrentWave.SpaceTruckCount = 0;
		CurrentWave.PlanetDestroyerCount = EnemiesToSpawnAtOnce;
		CurrentWave.Time = SpawnTimeInterval * 2.5;
	}
}

void AEnemySpawner::DestroyEnemyShips()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		AShip::StaticClass(),
		FoundActors);

	for (AActor* Actor : FoundActors) {
		if (!IsValid(Cast<APlanetDestroyer>(Actor))) {
			if (!Cast<AShip>(Actor)->bIsPlayer) {
				Actor->Destroy();
			}
		}
	}

}

void AEnemySpawner::CheckPlanetDestroyer()
{
	TArray<AActor*> FoundDestroyers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		APlanetDestroyer::StaticClass(),
		FoundDestroyers);

	if (FoundDestroyers.Num() <= 0) {
		CurrentWave.SpaceDartCount = 999;
		CurrentWave.SmartSpaceDartCount = 999;
		CurrentWave.SpaceArcherCount = 999;
		CurrentWave.SpaceTruckCount = 999;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
			&AEnemySpawner::SpawnEnemies,
			CurrentWave.Time, false, CurrentWave.Time);
	}
}

void AEnemySpawner::CheckWaveFinished()
{
	TArray<AActor*> FoundShips;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), 
										AShip::StaticClass(), 
										FoundShips);

	if (FoundShips.Num() <= PlayerShipsCount) {
		if (CurrWaveCount + 1 < TotalWaves) {
			CurrWaveCount++;
			TransferWaveData(Waves[CurrWaveCount]);
			GetWorldTimerManager().ClearTimer(MemberTimerHandle);
			SetWaveTimer(Waves[CurrWaveCount]->Time);
		}
		else {
			bLevelFinished = true;
		}
	}
}

void AEnemySpawner::TransferWaveData(FWaveStruct* Wave)
{
	CurrentWave.SpaceDartCount = Wave->SpaceDartCount;
	CurrentWave.SmartSpaceDartCount = Wave->SmartSpaceDartCount;
	CurrentWave.SpaceArcherCount = Wave->SpaceArcherCount;
	CurrentWave.SpaceTruckCount = Wave->SpaceTruckCount;
	CurrentWave.PlanetDestroyerCount = Wave->PlanetDestroyerCount;
	CurrentWave.Time = Wave->Time;
}

int AEnemySpawner::GetCurrWaveCount()
{
	return CurrWaveCount;
}
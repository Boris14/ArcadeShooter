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

	if(Levels.Num() < 1 && IsValid(Level1) && IsValid(Level2) &&
		IsValid(Level3) && IsValid(Level4) && IsValid(Level5)) {
		Levels.Add(Level1);
		Levels.Add(Level2);
		Levels.Add(Level3);
		Levels.Add(Level4);
		Levels.Add(Level5);
		StartWorking();
	}

}

void AEnemySpawner::StartWorking()
{
	const FString Context(TEXT("Wave"));

	FString WaveRowIndex = "";

	for (int i = 1; ; ++i) {
		WaveRowIndex = "Wave" + FString::FromInt(i);
		FWaveStruct* WaveRow = (Levels[CurrentLevel]->FindRow<FWaveStruct>(FName(WaveRowIndex), Context, true));

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

void AEnemySpawner::SetWaveTimer(float WaveTime)
{
	WaveEnemiesLeft = Waves[CurrWaveCount]->SpaceDartCount +
		Waves[CurrWaveCount]->SmartSpaceDartCount +
		Waves[CurrWaveCount]->SpaceArcherCount +
		Waves[CurrWaveCount]->SpaceTruckCount + 
		Waves[CurrWaveCount]->PlanetDestroyerCount;
	
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawner::SpawnEnemy, WaveTime / WaveEnemiesLeft, true, 10.0f);
}

void AEnemySpawner::SpawnEnemy()
{
	float Angle = FMath::RandRange(0.0f, 360.0f);
	float X = sin(Angle * (PI / 180)) * SpawnDistance;
	float Y = cos(Angle * (PI / 180)) * SpawnDistance;

	FVector SpawnPoint = FVector(X, Y, 0);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, 
															FVector(0, 0, 0));
	SpawnRotation.Yaw = 270 - Angle;

	AShip* SpawnedShip = nullptr;
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
	else {
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, 
										&AEnemySpawner::CheckWaveFinished, 
										1.0f, true, 1.0f);
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
}

int AEnemySpawner::GetCurrWaveCount()
{
	return CurrWaveCount;
}
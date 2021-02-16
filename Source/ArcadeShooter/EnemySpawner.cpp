// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnDistance = 4000;

	static ConstructorHelpers::FObjectFinder<UDataTable> Level1Object(TEXT("DataTable'/Game/Data/Level1'"));
	if (Level1Object.Succeeded()) {
		Level1 = Level1Object.Object;
	}

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	const FString Context(TEXT("Wave"));

	Waves.Add(Level1->FindRow<FWaveStruct>(FName(TEXT("Wave1")), Context, true));
	Waves.Add(Level1->FindRow<FWaveStruct>(FName(TEXT("Wave2")), Context, true));
	Waves.Add(Level1->FindRow<FWaveStruct>(FName(TEXT("Wave3")), Context, true));
	Waves.Add(Level1->FindRow<FWaveStruct>(FName(TEXT("Wave4")), Context, true));

	CurrWaveCount = 0;
	TransferWaveData(Waves[CurrWaveCount]);

	SetWaveTimer(Waves[CurrWaveCount]->Time);
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SetWaveTimer(float WaveTime)
{
	WaveEnemiesLeft = Waves[CurrWaveCount]->SpaceDartCount +
		Waves[CurrWaveCount]->SmartSpaceDartCount +
		Waves[CurrWaveCount]->SpaceArcherCount +
		Waves[CurrWaveCount]->SpaceTruckCount;
	
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawner::SpawnEnemy, WaveTime / WaveEnemiesLeft, true, 10.0f);
}

void AEnemySpawner::SpawnEnemy()
{
	float Angle = FMath::RandRange(0.0f, 360.0f);
	float X = sin(Angle * (PI / 180)) * SpawnDistance;
	float Y = cos(Angle * (PI / 180)) * SpawnDistance;

	FVector SpawnPoint = FVector(X, Y, 0);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, FVector(0, 0, 0));
	SpawnRotation.Yaw = 270 - Angle;

	AActor* SpawnedActor = nullptr;

	int32 GeneratedNum;

	bool bHasEnemy = false;

	while (!bHasEnemy && WaveEnemiesLeft > 0) {

		GeneratedNum = FMath::RandRange(0, 3);

		if (GeneratedNum == 0 && CurrentWave.SpaceDartCount > 0) {
			SpawnedActor = GetWorld()->SpawnActor<AShip>(SpaceDartClass, SpawnPoint, SpawnRotation);
			CurrentWave.SpaceDartCount--;
			bHasEnemy = true;
		}
		else if (GeneratedNum == 1 && CurrentWave.SmartSpaceDartCount > 0) {
			SpawnedActor = GetWorld()->SpawnActor<AShip>(SmartSpaceDartClass, SpawnPoint, SpawnRotation);
			CurrentWave.SmartSpaceDartCount--;
			bHasEnemy = true;
		}
		else if (GeneratedNum == 2 && CurrentWave.SpaceArcherCount > 0) {
			SpawnedActor = GetWorld()->SpawnActor<AShip>(SpaceArcherClass, SpawnPoint, SpawnRotation);
			CurrentWave.SpaceArcherCount--;
			bHasEnemy = true;
		}
		else if (CurrentWave.SpaceTruckCount > 0) {
			SpawnedActor = GetWorld()->SpawnActor<AShip>(SpaceTruckClass, SpawnPoint, SpawnRotation);
			CurrentWave.SpaceTruckCount--;
			bHasEnemy = true;
		}

		if (bHasEnemy) {
			WaveEnemiesLeft--;
		}
	}

	if (bHasEnemy && SpawnedActor != nullptr) {
		AShip* SpawnedShip = Cast<AShip>(SpawnedActor);

		if (SpawnedShip != nullptr) {
			SpawnedShip->Initialize(Angle);
		}
	}
	else {
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawner::CheckWaveFinished, 1.0f, true, 1.0f);
	}
}

void AEnemySpawner::CheckWaveFinished()
{
	TArray<AActor*> FoundShips;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShip::StaticClass(), FoundShips);

	if (FoundShips.Num() <= 2) {
		CurrWaveCount++;
		if (CurrWaveCount <= 3) {
			TransferWaveData(Waves[CurrWaveCount]);
			GetWorldTimerManager().ClearTimer(MemberTimerHandle);
			SetWaveTimer(Waves[CurrWaveCount]->Time);
		}
	}
}

void AEnemySpawner::TransferWaveData(FWaveStruct* Wave)
{
	CurrentWave.SpaceDartCount = Wave->SpaceDartCount;
	CurrentWave.SmartSpaceDartCount = Wave->SmartSpaceDartCount;
	CurrentWave.SpaceArcherCount = Wave->SpaceArcherCount;
	CurrentWave.SpaceTruckCount = Wave->SpaceTruckCount;
}
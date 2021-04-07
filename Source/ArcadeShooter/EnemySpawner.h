// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship.h"
#include "WaveStruct.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "EnemySpawner.generated.h"

UCLASS(Blueprintable)
class ARCADESHOOTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float SpawnDistance;

	FTimerHandle MemberTimerHandle;

	TArray<FWaveStruct*> Waves;

	FWaveStruct CurrentWave = FWaveStruct();

	float WaveEnemiesLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrWaveCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerShipsCount = 0;

	int TotalWaves = 0;

	bool bLevelFinished = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentLevel;

	void StartWorking();

	int GetCurrWaveCount();

	void SpawnEnemy();

	void CheckWaveFinished();

	void SetWaveTimer(float WaveTime);

	void TransferWaveData(FWaveStruct* Wave);

private:
	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SpaceDartClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SmartSpaceDartClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SpaceArcherClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SpaceTruckClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> PlanetDestroyerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* Level1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* Level2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* Level3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* Level4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* Level5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UDataTable*> Levels;
};

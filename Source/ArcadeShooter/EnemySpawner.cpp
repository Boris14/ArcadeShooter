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

	FWaveStruct* Wave1 = Level1->FindRow<FWaveStruct>(FName(TEXT("Wave1")), Context, true);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Wave1 Space Darts: %d"), Wave1->SpaceDartCount));

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemySpawner::SpawnEnemy, 3.0f, true, 2.0f);
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy()
{
	float Angle = FMath::RandRange(0.0f, 360.0f);
	float X = sin(Angle * (PI / 180)) * SpawnDistance;
	float Y = cos(Angle * (PI / 180)) * SpawnDistance;

	FVector SpawnPoint = FVector(X, Y, 0);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, FVector(0, 0, 0));
	SpawnRotation.Yaw = 270 - Angle;

	AActor* SpawnedActor;

	int32 GeneratedNum = FMath::RandRange(0, 3);

	if (GeneratedNum == 0) {
		SpawnedActor = GetWorld()->SpawnActor<AShip>(SpaceDartClass, SpawnPoint, SpawnRotation);
	}
	else if(GeneratedNum == 1){
		SpawnedActor = GetWorld()->SpawnActor<AShip>(SmartSpaceDartClass, SpawnPoint, SpawnRotation);
	}
	else if(GeneratedNum == 2){
		SpawnedActor = GetWorld()->SpawnActor<AShip>(SpaceTruckClass, SpawnPoint, SpawnRotation);
	}
	else {
		SpawnedActor = GetWorld()->SpawnActor<AShip>(SpaceArcherClass, SpawnPoint, SpawnRotation);
	}

	AShip* SpawnedShip = Cast<AShip>(SpawnedActor);

	if (SpawnedShip != nullptr) {
		SpawnedShip->Initialize(Angle);
	}
}
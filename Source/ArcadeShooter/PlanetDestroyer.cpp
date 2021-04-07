// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetDestroyer.h"

APlanetDestroyer::APlanetDestroyer()
{
	MeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent2"));
	MeshComponent2->SetupAttachment(RootComponent);

	MeshComponent3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent3"));
	MeshComponent3->SetupAttachment(RootComponent);
}

void APlanetDestroyer::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth = Health;
}

void APlanetDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((CurrentState == State::MovingAroundAndShooting || 
		CurrentState == State::MovingAroundAndSpawning) && 
		Speed == NormalSpeed) {
		Speed = NormalSpeed / 1.5;
	}
	else if (Speed == SlowedSpeed) {
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AShip::SetNormalSpeed, 2, false, 2);
	}
	else if (Speed < NormalSpeed && 
		CurrentState != State::MovingAroundAndShooting && 
		CurrentState != State::MovingAroundAndSpawning) {
		Speed = NormalSpeed;
	}

}

APlanetDestroyer* APlanetDestroyer::DetachArm(bool bIsLeftArm)
{
	APlanetDestroyer* SpawnedArm = nullptr;

	if (IsValid(MeshComponent) && bIsLeftArm) {
		MeshComponent->DestroyComponent();
	}
	else if (IsValid(MeshComponent2)) {
		MeshComponent2->DestroyComponent();
	}

	ChangeState(State::Stopped);
	ArmsCount--;

	SpawnedArm = GetWorld()->SpawnActor<APlanetDestroyer>(PlanetDestroyerClass, 
		GetActorLocation() + GetActorForwardVector() * 170, GetActorRotation());

	if (IsValid(SpawnedArm)) {
		SpawnedArm->bIsBody = false;
		SpawnedArm->bLeftArm = bIsLeftArm;
		SpawnedArm->Initialize(Angle);
		SpawnedArm->InitializeArm();
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	}

	PlayDetachSound();

	return SpawnedArm;
}

void APlanetDestroyer::SpawnShip()
{
	UClass* ShipClass = nullptr;
	AShip* SpawnedShip = nullptr;

	switch (FMath::RandRange(0, 2)) {
	case 0:
		ShipClass = SpaceDartClass;
		break;
	
	case 1:
		ShipClass = SmartSpaceDartClass;
		break;

	case 2:
		ShipClass = SpaceArcherClass;
		break;

	default:
		break;
	}

	SpawnedShip = GetWorld()->SpawnActor<AShip>(ShipClass, 
		GetActorLocation() + (GetActorForwardVector() * 170), 
		GetActorRotation());

	if (IsValid(SpawnedShip)) {
		SpawnedShip->Initialize(Angle);
	}
}

void APlanetDestroyer::Fire()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(EnemyProjectileClass,
		FindProjectileSpawnLocation(),
		GetActorRotation());

	if (IsValid(Projectile)) {
		Projectile->SetParameters(GetController(), 0);
	}
}

void APlanetDestroyer::InitializeArm()
{
	Health = MaxHealth / 34;
	Score *= 0.3;
	MeshComponent3->DestroyComponent();
	if (bLeftArm) {
		MeshComponent2->DestroyComponent();
	}
	else {
		MeshComponent->DestroyComponent();
	}
}

void APlanetDestroyer::ChangeState(TEnumAsByte<State> NewState)
{
	CurrentState = NewState;
}

void APlanetDestroyer::IncreaseRage()
{
	Rage++;
	if (Rage == 2) {
		NormalSpeed *= 1.3;
		FireRate *= 0.6;
	}
	else {
		NormalSpeed *= 1.5;
		FireRate *= 0.5;
	}
}

void APlanetDestroyer::ShowCharge(bool bForShooting)
{
	if (bForShooting) {
		ShowShootingCharge(FindProjectileSpawnLocation());
	}
	else {
		ShowSpawnCharge(FindProjectileSpawnLocation());
	}
}


FVector APlanetDestroyer::FindProjectileSpawnLocation()
{
	FVector SpawnLocation;

	if (!bIsBody) {
		UStaticMeshComponent* ArmComponent = nullptr;

		if (bLeftArm) {
			ArmComponent = MeshComponent;
		}
		else {
			ArmComponent = MeshComponent2;
		}

		SpawnLocation = ArmComponent->GetComponentLocation() + GetActorForwardVector() * 250;
		SpawnLocation.Z = 0;
	}
	else {
		SpawnLocation = GetActorLocation() + GetActorForwardVector() * 170;
	}

	return SpawnLocation;
}

void APlanetDestroyer::ShowArmExplosion(bool bIsLeftArm)
{
	if (bIsLeftArm) {
		ShowExplosion(MeshComponent->GetComponentLocation());
	}
	else {
		ShowExplosion(MeshComponent2->GetComponentLocation());
	}
}

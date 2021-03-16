// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceArcherController.h"

ASpaceArcherController::ASpaceArcherController()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_SpaceArcherProjectile'"));

	if (ProjectileBlueprint.Object) {
		ProjectileClass = (UClass*)ProjectileBlueprint.Object->GeneratedClass;
	}
}

void ASpaceArcherController::BeginPlay()
{
	Super::BeginPlay();
}

void ASpaceArcherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShip* SpaceArcher = Cast<AShip>(GetPawn());

	if (IsValid(SpaceArcher)) {

		float Dist = FVector::Dist(SpaceArcher->GetActorLocation(), FVector(0, 0, 0));

		if (Dist <= FireDistance && !bIsFiring) {
			bIsFiring = true;
			GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASpaceArcherController::Fire, FireRate, true, FireRate);
		}
		else if(!bIsFiring){
			SpaceArcher->SetActorLocation(SpaceArcher->GetActorLocation() + 
				(SpaceArcher->GetActorForwardVector() * SpaceArcher->Speed * DeltaTime));
		}
	}
}

void ASpaceArcherController::Fire()
{
	APawn* ControlledPawn = GetPawn();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
		ControlledPawn->GetActorLocation() + (ControlledPawn->GetActorForwardVector() * 170),
		ControlledPawn->GetActorRotation());

	if (Projectile) {
		Projectile->Initialize(this, 0);
	}
}
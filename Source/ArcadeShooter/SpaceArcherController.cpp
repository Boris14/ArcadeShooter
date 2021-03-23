// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceArcherController.h"

ASpaceArcherController::ASpaceArcherController()
{
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
	AShip* SpaceArcher = Cast<AShip>(GetPawn());

	if (IsValid(SpaceArcher)) {
		SpaceArcher->Fire();
	}
}
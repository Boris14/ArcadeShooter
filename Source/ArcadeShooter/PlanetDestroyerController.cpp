// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetDestroyerController.h"

APlanetDestroyerController::APlanetDestroyerController() 
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlanetDestroyerController::BeginPlay()
{
	Super::BeginPlay();

}

void APlanetDestroyerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlanetDestroyer* PlanetDestroyer = Cast<APlanetDestroyer>(GetPawn());

	if (IsValid(PlanetDestroyer)) {

		float Dist = FVector::Dist(PlanetDestroyer->GetActorLocation(),
			FVector(0, 0, 0));

		if (Dist <= FireDistance) {
			if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
				GetWorldTimerManager().SetTimer(MemberTimerHandle, this, 
					&APlanetDestroyerController::SwitchAction, 2.5, true, 2.5);
			
			}
			if (bIsMoving) {
				MoveAroundPlanet(PlanetDestroyer, DeltaTime);
			}
			else {

			}
		}
		else {
			PlanetDestroyer->SetActorLocation(PlanetDestroyer->GetActorLocation() +
				(PlanetDestroyer->GetActorForwardVector() * PlanetDestroyer->Speed * DeltaTime));
		}
	}

}

void APlanetDestroyerController::MoveAroundPlanet(APlanetDestroyer* ControlledDestroyer, float DeltaTime)
{
	FVector DestroyerLocation = ControlledDestroyer->GetActorLocation();
	FRotator DestroyerRotation = ControlledDestroyer->GetActorRotation();

	if (bMoveClockwise) {
		ControlledDestroyer->Angle = ControlledDestroyer->Angle - 
									(ControlledDestroyer->Speed * 0.05 * DeltaTime);
	}
	else {
		ControlledDestroyer->Angle = ControlledDestroyer->Angle + 
									(ControlledDestroyer->Speed * 0.05 * DeltaTime);
	}

	DestroyerLocation.X = sin(ControlledDestroyer->Angle * (PI / 180)) * FireDistance;
	DestroyerLocation.Y = cos(ControlledDestroyer->Angle * (PI / 180)) * FireDistance;

	DestroyerRotation.Yaw = 270 - ControlledDestroyer->Angle;

	ControlledDestroyer->SetActorLocation(DestroyerLocation);
	ControlledDestroyer->SetActorRotation(DestroyerRotation);

}

void APlanetDestroyerController::SwitchAction()
{
	bIsMoving = !bIsMoving;
	if (bIsMoving) {
		bMoveClockwise = FMath::RandBool();
	}
}
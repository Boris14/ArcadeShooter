// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceTruckParticleController.h"


ASpaceTruckParticleController::ASpaceTruckParticleController()
{
	
}

void ASpaceTruckParticleController::BeginPlay()
{
	Super::BeginPlay();
}


void ASpaceTruckParticleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShip* SpaceParticle = Cast<AShip>(GetPawn());

	if (IsValid(SpaceParticle)) {

		if (ExplosionTime > 0) {
			if (DeltaTime > ExplosionTime) {
				DeltaTime = ExplosionTime;
			}
			ExplosionTime -= DeltaTime;
			ParticleSpeed -= (DeltaTime * SpeedChange);
		}
		else if (!bExplosionEnded) {
			bExplosionEnded = true;
			GetPawn()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(
												GetPawn()->GetActorLocation(), 
												FVector(0, 0, 0)
			));
		}
		else if (ParticleSpeed < SpaceParticle->Speed) {
			ParticleSpeed += (DeltaTime * SpeedChange);
			if (ParticleSpeed > SpaceParticle->Speed) {
				ParticleSpeed = SpaceParticle->Speed;
			}
		}

		SpaceParticle->SetActorLocation(SpaceParticle->GetActorLocation() +
			(SpaceParticle->GetActorForwardVector() *
				ParticleSpeed * DeltaTime));
	}
}

void ASpaceTruckParticleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AShip* SpaceParticle = Cast<AShip>(InPawn);

	if (IsValid(SpaceParticle)) {
		ParticleSpeed = SpaceParticle->Speed * 5;
		ExplosionTime = 0.5;
		SpeedChange = ParticleSpeed / ExplosionTime;

		FRotator PawnRotation = InPawn->GetActorRotation();
		PawnRotation.Yaw = FMath::RandRange(0, 360);
		InPawn->SetActorRotation(PawnRotation);
	}
}
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

		if (bIsExploding) {
			SpaceParticle->SetActorLocation(SpaceParticle->GetActorLocation() + (SpaceParticle->GetActorForwardVector() * ParticleSpeed));
			ParticleSpeed *= 0.97;
		}
		else {
			SpaceParticle->SetActorLocation(SpaceParticle->GetActorLocation() + (SpaceParticle->GetActorForwardVector() * ParticleSpeed));
			if (ParticleSpeed < SpaceParticle->Speed * 2) {
				ParticleSpeed *= 1.02;
			}
		}
	}

}

void ASpaceTruckParticleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AShip* SpaceParticle = Cast<AShip>(InPawn);

	if (IsValid(SpaceParticle)) {
		ParticleSpeed = SpaceParticle->GetSpeed() * 12;
	}

	FRotator PawnRotation = InPawn->GetActorRotation();

	PawnRotation.Yaw = FMath::RandRange(0, 360);

	InPawn->SetActorRotation(PawnRotation);

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASpaceTruckParticleController::EndExplosion, 0.8, false, 0.8);
}

void ASpaceTruckParticleController::EndExplosion()
{
	bIsExploding = false;

	GetPawn()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), FVector(0, 0, 0)));
}
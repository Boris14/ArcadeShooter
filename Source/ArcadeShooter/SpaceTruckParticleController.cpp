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

		SpaceParticle->SetActorLocation(SpaceParticle->GetActorLocation() +
			(SpaceParticle->GetActorForwardVector() *
				ParticleSpeed * DeltaTime));

		if (bIsExploding) {
			ParticleSpeed *= 0.95;
		}
		else if (ParticleSpeed < SpaceParticle->Speed) {
			ParticleSpeed *= 1.07;
		}
	}
}

void ASpaceTruckParticleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AShip* SpaceParticle = Cast<AShip>(InPawn);

	if (IsValid(SpaceParticle)) {
		ParticleSpeed = SpaceParticle->Speed * 7;
	}

	FRotator PawnRotation = InPawn->GetActorRotation();

	PawnRotation.Yaw = FMath::RandRange(0, 360);

	InPawn->SetActorRotation(PawnRotation);

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASpaceTruckParticleController::EndExplosion, 0.6, false, 0.6);
}

void ASpaceTruckParticleController::EndExplosion()
{
	bIsExploding = false;

	GetPawn()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), FVector(0, 0, 0)));
}
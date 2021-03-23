// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartSpaceDartController.h"


ASmartSpaceDartController::ASmartSpaceDartController() 
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASmartSpaceDartController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASmartSpaceDartController::SwitchCanAvoid, 1, true, 1);
}

void ASmartSpaceDartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShip* SpaceDart = Cast<AShip>(GetPawn());

	if (IsValid(SpaceDart)) {

		if (bCanAvoid) {
			FHitResult OutHit;
			FCollisionQueryParams Parameters;
			FCollisionResponseParams ResponseParams;

			GetWorld()->LineTraceSingleByChannel(OutHit, SpaceDart->GetActorLocation(),
				SpaceDart->GetActorForwardVector(), ECC_Pawn,
				Parameters, ResponseParams);

			APlanet* HitPlanet = Cast<APlanet>(OutHit.GetActor());

			if (!IsValid(HitPlanet)) {
				AvoidPlayer(SpaceDart, DeltaTime);
			}
		}

		SpaceDart->SetActorLocation(SpaceDart->GetActorLocation() + 
									(SpaceDart->GetActorForwardVector() * 
										SpaceDart->Speed * DeltaTime));
	}
}

void ASmartSpaceDartController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASmartSpaceDartController::AvoidPlayer(AShip* SpaceDart, float DeltaTime)
{
	float Dist = FVector::Dist(SpaceDart->GetActorLocation(), FVector(0, 0, 0));
	FVector ShipLocation = SpaceDart->GetActorLocation();
	FRotator ShipRotation = SpaceDart->GetActorRotation();

	if (bAvoidClockwise) {
		SpaceDart->Angle = SpaceDart->Angle - (SpaceDart->Speed * 0.05 * DeltaTime);
	}
	else {
		SpaceDart->Angle = SpaceDart->Angle + (SpaceDart->Speed * 0.05 * DeltaTime);
	}

	ShipLocation.X = sin(SpaceDart->Angle * (PI / 180)) * Dist;
	ShipLocation.Y = cos(SpaceDart->Angle * (PI / 180)) * Dist;

	ShipRotation.Yaw = 270 - SpaceDart->Angle;

	SpaceDart->SetActorLocation(ShipLocation);
	SpaceDart->SetActorRotation(ShipRotation);
}

void ASmartSpaceDartController::SwitchCanAvoid()
{
	bCanAvoid = !bCanAvoid;
	bAvoidClockwise = FMath::RandBool();
}

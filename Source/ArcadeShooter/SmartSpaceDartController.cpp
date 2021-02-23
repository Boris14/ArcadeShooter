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

		FHitResult OutHit;
		FCollisionQueryParams Parameters;
		FCollisionResponseParams ResponseParams;

		GetWorld()->LineTraceSingleByChannel(OutHit, SpaceDart->GetActorLocation(), 
											SpaceDart->GetActorForwardVector(), ECC_Pawn, 
											Parameters, ResponseParams);

		APlanet* HitPlanet = Cast<APlanet>(OutHit.GetActor());

		if (!IsValid(HitPlanet)) {
			if (bCanAvoid) {
				AvoidPlayer(SpaceDart);
			}
		}

		SpaceDart->SetActorLocation(SpaceDart->GetActorLocation() + 
									(SpaceDart->GetActorForwardVector() * SpaceDart->Speed * 2));
	}
}

void ASmartSpaceDartController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASmartSpaceDartController::AvoidPlayer(AShip* SpaceDart)
{
	float Dist = FVector::Dist(SpaceDart->GetActorLocation(), FVector(0, 0, 0));
	FVector ShipLocation = SpaceDart->GetActorLocation();
	FRotator ShipRotation = SpaceDart->GetActorRotation();

	if (bAvoidClockwise) {
		SpaceDart->Angle = SpaceDart->Angle - (SpaceDart->Speed * 0.15);
	}
	else {
		SpaceDart->Angle = SpaceDart->Angle + (SpaceDart->Speed * 0.15);
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

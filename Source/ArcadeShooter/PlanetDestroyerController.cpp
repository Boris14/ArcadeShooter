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

	for (int i = 0; i < DetachedArms.Num(); ++i) {
		if (!IsValid(DetachedArms[i])) {
			DetachedArms.RemoveAt(i);
		}
	}
	
	APlanetDestroyer* PlanetDestroyer = Cast<APlanetDestroyer>(GetPawn());

	if (IsValid(PlanetDestroyer)) {

		if (PlanetDestroyer->bIsBody) {
			if(PlanetDestroyer->Health < PlanetDestroyer->MaxHealth * 0.6 &&
				PlanetDestroyer->ArmsCount > 1) {
				PlanetDestroyer->ShowArmExplosion(true);
				DetachedArms.Push(PlanetDestroyer->DetachArm(true));
			}
			else if (PlanetDestroyer->Health < PlanetDestroyer->MaxHealth * 0.3 &&
				PlanetDestroyer->ArmsCount > 0) {
				PlanetDestroyer->ShowArmExplosion(false);
				DetachedArms.Push(PlanetDestroyer->DetachArm(false));
			}

			if ((DetachedArms.Num() + PlanetDestroyer->ArmsCount < 2 && PlanetDestroyer->Rage < 2) ||
				(DetachedArms.Num() + PlanetDestroyer->ArmsCount < 1 && PlanetDestroyer->Rage < 3)) {
				PlanetDestroyer->IncreaseRage();
				ActionsLeft = PlanetDestroyer->ArmsCount + 1;
			}
		}

		switch (PlanetDestroyer->CurrentState) {
			case State::MovingTowardsPlanet:
				if (FVector::Dist(PlanetDestroyer->GetActorLocation(),
					FVector(0, 0, 0)) <= PlanetDestroyer->FireDistance) {
					SwitchStates();
				}
				PlanetDestroyer->SetActorLocation(PlanetDestroyer->GetActorLocation() +
					(PlanetDestroyer->GetActorForwardVector() * PlanetDestroyer->Speed * DeltaTime));

				break;

			case State::MovingAroundPlanet:
				MoveAroundPlanet(PlanetDestroyer, DeltaTime);
				break;

			case State::Shooting:
				if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
					PlanetDestroyer->PlayChargingSound();
					PlanetDestroyer->ShowCharge(true);
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::DoAction, PlanetDestroyer->FireRate * 2, false, PlanetDestroyer->FireRate * 2);
				}
				break;

			case State::MovingAroundAndSpawning:
				if (ActionsLeft < PlanetDestroyer->ArmsCount + 1) {
					MoveAroundPlanet(PlanetDestroyer, DeltaTime);
				}
				if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
					if (PlanetDestroyer->Rage < 3) {
						PlanetDestroyer->PlayChargingSound();
						PlanetDestroyer->ShowCharge(false);
					}
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::DoAction, PlanetDestroyer->FireRate * 4, false, PlanetDestroyer->FireRate * 4);
				}
				break;

			case State::MovingAroundAndShooting:
				if (ActionsLeft < PlanetDestroyer->ArmsCount + 1) {
					MoveAroundPlanet(PlanetDestroyer, DeltaTime);
				}
				if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
					if (PlanetDestroyer->Rage < 3) {
						PlanetDestroyer->PlayChargingSound();
						PlanetDestroyer->ShowCharge(true);
					}
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::DoAction, PlanetDestroyer->FireRate * 4, false, PlanetDestroyer->FireRate * 4);
				}
				break;

			case State::Stopped:
				if (!bStoppedTimerOn) {
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::DoAction, 3, false, 3);
					bStoppedTimerOn = true;
				}
				break;

			default:
				break;
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

	DestroyerLocation.X = sin(ControlledDestroyer->Angle * (PI / 180)) * ControlledDestroyer->FireDistance;
	DestroyerLocation.Y = cos(ControlledDestroyer->Angle * (PI / 180)) * ControlledDestroyer->FireDistance;

	DestroyerRotation.Yaw = 270 - ControlledDestroyer->Angle;

	ControlledDestroyer->SetActorLocation(DestroyerLocation);
	ControlledDestroyer->SetActorRotation(DestroyerRotation);

}

void APlanetDestroyerController::SwitchStates()
{
	APlanetDestroyer* PlanetDestroyer = Cast<APlanetDestroyer>(GetPawn());

	if (IsValid(PlanetDestroyer)) {
		switch (PlanetDestroyer->CurrentState) {
			case State::MovingAroundPlanet:
				if (PlanetDestroyer->bIsBody) {
					if (FMath::RandBool() && PlanetDestroyer->Rage < 3) {
						PlanetDestroyer->ChangeState(State::MovingAroundAndSpawning);
					}
					else {
						bMoveClockwise = FMath::RandBool();
						PlanetDestroyer->ChangeState(State::MovingAroundAndShooting);
					}
				}
				else {
					PlanetDestroyer->ChangeState(State::Shooting);
				}
				break;

			default:
				bMoveClockwise = FMath::RandBool();
				PlanetDestroyer->ChangeState(State::MovingAroundPlanet);
				GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
					&APlanetDestroyerController::SwitchStates, PlanetDestroyer->FireRate * 6, false, PlanetDestroyer->FireRate * 6);
				break;
		}
	}
}

void APlanetDestroyerController::DoAction()
{
	APlanetDestroyer* PlanetDestroyer = Cast<APlanetDestroyer>(GetPawn());

	if (IsValid(PlanetDestroyer)) {
		switch (PlanetDestroyer->CurrentState)
		{
			case State::Shooting:
				PlanetDestroyer->Fire();
				GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
					&APlanetDestroyerController::SwitchStates, PlanetDestroyer->FireRate * 2, false, PlanetDestroyer->FireRate * 2);
				break;

			case State::MovingAroundAndSpawning:
				if (ActionsLeft > 0) {
					PlanetDestroyer->SpawnShip();
					PlanetDestroyer->PlaySpawnSound();
					ActionsLeft--;
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::DoAction, PlanetDestroyer->FireRate, false, PlanetDestroyer->FireRate);
				}
				else {
					ActionsLeft = PlanetDestroyer->ArmsCount + 1;
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::SwitchStates, PlanetDestroyer->FireRate * 2, false, PlanetDestroyer->FireRate * 2);
				}
				break;

			case State::MovingAroundAndShooting:
				if (ActionsLeft > 0) {
					PlanetDestroyer->Fire();
					ActionsLeft--;
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::DoAction, PlanetDestroyer->FireRate, false, PlanetDestroyer->FireRate);
				}
				else {
					ActionsLeft = PlanetDestroyer->ArmsCount + 1;
					GetWorldTimerManager().SetTimer(MemberTimerHandle, this,
						&APlanetDestroyerController::SwitchStates, PlanetDestroyer->FireRate * 2, false, PlanetDestroyer->FireRate * 2);
				}
				break;

			case State::Stopped:
				bStoppedTimerOn = false;
				SwitchStates();
				break;
			default:
				break;
		}
	}
}
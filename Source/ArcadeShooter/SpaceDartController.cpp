// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceDartController.h"


ASpaceDartController::ASpaceDartController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpaceDartController::BeginPlay()
{
	Super::BeginPlay();
}

void ASpaceDartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShip* SpaceDart =	Cast<AShip>(GetPawn());

	if (IsValid(SpaceDart)) {
		SpaceDart->SetActorLocation(SpaceDart->GetActorLocation() + 
									(SpaceDart->GetActorForwardVector() * SpaceDart->Speed * 2));
	}
	else {
		ADrop* Drop = Cast<ADrop>(GetPawn());
		if (IsValid(Drop)) {
			Drop->SetActorLocation(Drop->GetActorLocation() + (Drop->GetActorForwardVector() * Drop->Speed));
		}
	}

}

void ASpaceDartController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Ship.h"
#include "TimerManager.h"
#include "SpaceTruckParticleController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API ASpaceTruckParticleController : public AAIController
{
	GENERATED_BODY()

public:
	ASpaceTruckParticleController();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	//void EndExplosion();
	
protected:

	FTimerHandle MemberTimerHandle;

	bool bExplosionEnded = false;

	float ParticleSpeed;

	float ExplosionTime = 0;

	float SpeedChange = 0;
};

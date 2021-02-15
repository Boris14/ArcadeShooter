// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ship.h"
#include "TimerManager.h"
#include "Planet.h"
#include "Engine/World.h"
#include "SmartSpaceDartController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API ASmartSpaceDartController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASmartSpaceDartController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void AvoidPlayer(AShip* SpaceDart);

	void SwitchCanAvoid();

public:
	FTimerHandle MemberTimerHandle;

	bool bCanAvoid = true;

	bool bAvoidClockwise = FMath::RandBool();
};

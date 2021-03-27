// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TimerManager.h"
#include "PlanetDestroyer.h"
#include "PlanetDestroyerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API APlanetDestroyerController : public AAIController
{
	GENERATED_BODY()
	
public:
	APlanetDestroyerController();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void MoveAroundPlanet(APlanetDestroyer* ControlledDestroyer, float DeltaTime);

	void SwitchAction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireDistance;

	bool bIsMoving = false;

	bool bMoveClockwise = false;

	FTimerHandle MemberTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ship.h"
#include "Drop.h"
#include "SpaceDartController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API ASpaceDartController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASpaceDartController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};

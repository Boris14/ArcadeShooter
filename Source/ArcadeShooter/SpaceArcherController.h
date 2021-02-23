// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "Ship.h"
#include "SpaceArcherController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API ASpaceArcherController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASpaceArcherController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Fire();

	FTimerHandle MemberTimerHandle;

	float FireDistance = 1600;

	int FireRate = 2;

	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> ProjectileClass;
};

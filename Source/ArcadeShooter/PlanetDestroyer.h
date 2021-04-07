// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "PlanetDestroyer.generated.h"

/**
 * 
 */
UENUM()
enum State
{
	MovingAroundPlanet UMETA(DisplayName = "MovingAroundPlanet"),
	MovingTowardsPlanet UMETA(DisplayName = "MovingTowardsPlanet"),
	MovingAroundAndShooting UMETA(DisplayName = "MovingAroundAndShooting"),
	MovingAroundAndSpawning UMETA(DisplayName = "MovingAroundAndSpawning"),
	Shooting UMETA(DisplayName = "Shooting"),
	Stopped UMETA(DisplayName = "Stopped")
};



UCLASS()
class ARCADESHOOTER_API APlanetDestroyer : public AShip
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APlanetDestroyer();

	virtual void Tick(float DeltaTime) override;

	APlanetDestroyer* DetachArm(bool bIsLeftArm);

	void SpawnShip();

	void InitializeArm();

	void IncreaseRage();

	virtual void Fire() override;

	FVector FindProjectileSpawnLocation();

	void ChangeState(TEnumAsByte<State> NewState);

	void ShowArmExplosion(bool bIsLeftArm);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowShootingCharge(FVector SpawnLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSpawnCharge(FVector SpawnLocation);

	void ShowCharge(bool bForShooting);

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySpawnSound();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayChargingSound();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDetachSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBody;

	bool bLeftArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Rage = 1;

	int ArmsCount = 2;

	float FireRate = 0.4;

	float MaxHealth = 0;

	float FireDistance = 1800;

	TEnumAsByte<State> CurrentState = State::MovingTowardsPlanet;

	FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SpaceDartClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SmartSpaceDartClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SpaceArcherClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlanetDestroyer> PlanetDestroyerClass;
};

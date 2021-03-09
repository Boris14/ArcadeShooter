// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"
#include "Planet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Gun.h"
#include "Indicator.h"
#include "GameplayTags.h"
#include "Projectile.h"
#include "Ship.generated.h"

UCLASS(Blueprintable)
class ARCADESHOOTER_API AShip : public APawn
{
	GENERATED_BODY()

public:
	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldShowBonusGP = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldShowBonusScore = false;

	bool bCanShoot = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Level = 0;

	FTimerHandle MemberTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGun* GunComponent;

	float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	float NormalSpeed;

	AIndicator* Indicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GalaxyPoints = 0; 

	void Initialize(float InitAngle);

	virtual void CalculateDead();

	void CalculateMovement(float AxisValue);

	void Fire();

	void Reload();

	void SetShootingSpeed();

	void SetNormalSpeed();

	void Slow(float Amount);

	UFUNCTION(BlueprintCallable)
	bool Upgrade();

	float GetSpeed();

	bool GetIsPlayer();

	float GetFireRate();

	void AcquireHealthDrop(int DropHealth);

	virtual float TakeDamage(float DamageAmount,
							FDamageEvent const& DamageEvent,
							AController* EventInstigator,
							AActor* DamageCauser) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void AcquireWeaponDrop(WeaponType Weapon);

	FVector CalculateIndicatorLocation();

	void SpawnIndicator();

	void UpdateIndicator();

	void DestroyIndicator();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeMaterial(WeaponType Weapon);

private:
	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> HealthDropClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> RapidWeaponDropClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> RadialWeaponDropClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> FrostWeaponDropClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> IndicatorClass;
};

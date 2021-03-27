// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Planet.h"
#include "PopUpMessage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Gun.h"
#include "Indicator.h"
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

	bool bCanShoot = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Level = 0;

	FTimerHandle MemberTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGun* GunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	float NormalSpeed;

	AIndicator* Indicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GalaxyPoints = 0; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float VolumeMultiplier = 1;

	void Initialize(float InitAngle);

	virtual void CalculateDead();

	void CalculateMovement(float AxisValue, float DeltaTime);

	void Fire();

	void Reload();

	void SetShootingSpeed();

	void SetNormalSpeed();

	void Slow(float Amount);

	UFUNCTION(BlueprintCallable)
	bool Upgrade();

	float GetSpeed();

	bool GetIsPlayer();

	UFUNCTION(BlueprintCallable)
	WeaponType GetWeaponType();

	float GetFireRate();

	bool AcquireHealthDrop(int DropHealth);

	virtual float TakeDamage(float DamageAmount,
							FDamageEvent const& DamageEvent,
							AController* EventInstigator,
							AActor* DamageCauser) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool AcquireWeaponDrop(WeaponType Weapon);

	FVector CalculateIndicatorLocation();

	void SpawnIndicator();

	void UpdateIndicator();

	UFUNCTION(BlueprintCallable)
	void DestroyIndicator();

	UFUNCTION(BlueprintCallable)
	void SpaceTruckExplode(float SpaceTruckAngle);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeMaterial(WeaponType Weapon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayDamageTakenSound();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayDestroySound();

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

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APopUpMessage> PopUpMessageClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AShip> SpaceTruckParticleClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> SpaceArcherProjectileClass;
};

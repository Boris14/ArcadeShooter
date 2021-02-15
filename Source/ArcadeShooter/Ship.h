// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"
#include "Gun.h"
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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayer = false;

	FTimerHandle MemberTimerHandle;

	UPROPERTY(VisibleAnywhere)
	UGun* GunComponent;

	float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	void Initialize(float InitAngle);

	virtual void CalculateDead();

	void CalculateMovement(float AxisValue);

	void Fire();

	void SetShootingSpeed();

	void SetNormalSpeed();

	void SetSlowedSpeed();

	float GetSpeed();

	bool GetIsPlayer();

	float GetFireRate();

	virtual float TakeDamage(float DamageAmount,
							FDamageEvent const& DamageEvent,
							AController* EventInstigator,
							AActor* DamageCauser) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void ChangeWeapon();
};

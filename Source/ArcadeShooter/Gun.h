// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Gun.generated.h"


UENUM()
enum WeaponType 
{
	Rapid UMETA(DisplayName = "Rapid"),
	Radial UMETA(DisplayName = "Radial"),
	Frost UMETA(DisplayName = "Frost")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADESHOOTER_API UGun : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initialize(TEnumAsByte<WeaponType> CurrentWeapon);

	float FireRate;

	void Fire();

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> RapidProjectileClass;	

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> RadialProjectileClass;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> FrostProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponType)
	TEnumAsByte<WeaponType> Weapon;
};

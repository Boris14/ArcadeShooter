// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

// Sets default values for this component's properties
UGun::UGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UBlueprint> RapidProjectileBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_RapidProjectile'"));

	if (RapidProjectileBlueprint.Object) {
		RapidProjectileClass = (UClass*)RapidProjectileBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> RadialProjectileBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_RadialProjectile'"));

	if (RadialProjectileBlueprint.Object) {
		RadialProjectileClass = (UClass*)RadialProjectileBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> FrostProjectileBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_FrostProjectile'"));

	if (FrostProjectileBlueprint.Object) {
		FrostProjectileClass = (UClass*)FrostProjectileBlueprint.Object->GeneratedClass;
	}
}

void UGun::Initialize(TEnumAsByte<WeaponType> CurrentWeapon)
{
	Weapon = CurrentWeapon;

	switch (CurrentWeapon)
	{
		case WeaponType::Rapid:
			FireRate = 0.15f;
			break;

		case WeaponType::Radial:
			FireRate = 0.25f;
			break;

		case WeaponType::Frost:
			FireRate = 0.4f;
			break;

		default:
			break;
	}
}


// Called when the game starts
void UGun::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UGun::Fire()
{
	AProjectile* Projectile;

	AController* ShooterController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	switch (Weapon) 
	{
		case WeaponType::Rapid:
			Projectile = GetWorld()->SpawnActor<AProjectile>(RapidProjectileClass,
				GetComponentLocation() + (GetForwardVector() * 170),
				GetComponentRotation());
			if (Projectile) {
				Projectile->Initialize(ShooterController);
			}
			break;

		case WeaponType::Radial:
			Projectile = GetWorld()->SpawnActor<AProjectile>(RadialProjectileClass,
				GetComponentLocation() + (GetForwardVector() * 170),
				GetComponentRotation());
			if (Projectile) {
				Projectile->Initialize(ShooterController);
			}
			break;

		case WeaponType::Frost:
			Projectile = GetWorld()->SpawnActor<AProjectile>(FrostProjectileClass,
				GetComponentLocation() + (GetForwardVector() * 170),
				GetComponentRotation());
			if (Projectile) {
				Projectile->Initialize(ShooterController);
			}
			break;

		default:
			break;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

// Sets default values for this component's properties
UGun::UGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGun::SetWeapon(TEnumAsByte<WeaponType> CurrentWeapon)
{
	Weapon = CurrentWeapon;

	switch (CurrentWeapon)
	{
		case WeaponType::Rapid:
			if (WeaponLevels.Num() > 1) {
				FireRate = WeaponLevels[Level]->RapidFireRate;
			}
			else {
				FireRate = 0.0f;
			}
			ProjectileClass = RapidProjectileClass;
			break;

		case WeaponType::Radial:
			FireRate = 0.25f;
			ProjectileClass = RadialProjectileClass;
			break;

		case WeaponType::Frost:
			FireRate = 0.4f;
			ProjectileClass = FrostProjectileClass;
			break;

		default:
			break;
	}
}

void UGun::Initialize() 
{
	const FString Context(TEXT("WeaponLevels"));
	for (int i = 1; i <= 3; ++i) {
		FString RowIndex = "Level" + FString::FromInt(i);
		WeaponLevels.Add(WeaponData->FindRow<FWeaponStruct>(FName(RowIndex), Context, true));
	}
	SetWeapon(Weapon);
}

void UGun::Upgrade()
{
	if (Level < 2) {
		Level++;
		if (Weapon == WeaponType::Rapid) {
			FireRate = WeaponLevels[Level]->RapidFireRate;
		}
	}
}


// Called when the game starts
void UGun::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WeaponLevels.Num() < 1 && IsValid(WeaponData)) {
		Initialize();
	}
	// ...
}


void UGun::Fire()
{
	AProjectile* Projectile;
	AController* ShooterController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
		GetComponentLocation() + (GetForwardVector() * 170),
		GetComponentRotation());

	if (IsValid(Projectile)) {
		Projectile->SetParameters(ShooterController, Level);
	}
}

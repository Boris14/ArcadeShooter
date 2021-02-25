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

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataObject(TEXT("DataTable'/Game/Data/WeaponLevels'"));
	if (WeaponDataObject.Succeeded()) {
		WeaponData = WeaponDataObject.Object;
	}

	const FString Context(TEXT("Weapon"));
	for (int i = 1; i <= 3; ++i) {
		FString RowIndex = "Level" + FString::FromInt(i);
		WeaponLevels.Add(WeaponData->FindRow<FWeaponStruct>(FName(RowIndex), Context, true));
	}
}

void UGun::Initialize(TEnumAsByte<WeaponType> CurrentWeapon)
{
	Weapon = CurrentWeapon;

	switch (CurrentWeapon)
	{
		case WeaponType::Rapid:
			FireRate = WeaponLevels[Level]->RapidFireRate;
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
		Projectile->Initialize(ShooterController, Level);
	}
}

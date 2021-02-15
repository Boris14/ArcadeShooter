// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	GunComponent = CreateDefaultSubobject<UGun>(TEXT("GunComponent"));

	GunComponent->SetupAttachment(RootComponent);

	GunComponent->Initialize(WeaponType::Rapid);
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	if (bIsPlayer) {
		this->Tags.Add(FName("Player"));
		Angle = 0;
	}
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShip::Initialize(float InitAngle)
{
	Angle = InitAngle;
}

void AShip::CalculateDead()
{
	if (Health <= 0) {
		Destroy();
	}
}

void AShip::CalculateMovement(float AxisValue)
{
	Angle = Angle - (AxisValue * Speed);

	FVector ActorLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	ActorLocation.X = sin(Angle * (PI / 180)) * 721;
	ActorLocation.Y = cos(Angle * (PI / 180)) * 721; //721 planet radius

	ActorRotation.Yaw = 90 - Angle;

	SetActorLocation(ActorLocation);
	SetActorRotation(ActorRotation);
}

void AShip::Fire()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fire")));

	GunComponent->Fire();
	
}

float AShip::GetSpeed() {
	return Speed;
}

bool AShip::GetIsPlayer()
{
	return bIsPlayer;
}

void AShip::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bIsPlayer) {
		if (!OtherActor->ActorHasTag("Projectile") && !OtherActor->ActorHasTag("Enemy")) {
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(HitDamage, DamageEvent, GetController(), this);
			Destroy();
		}
	}
}

float AShip::TakeDamage(float DamageAmount,
						FDamageEvent const& DamageEvent,
						AController* EventInstigator,
						AActor* DamageCauser)
{
	if (bIsPlayer) 
	{
		Health = Health - DamageAmount;
		CalculateDead();
	}
	else 
	{
		AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
		if (Projectile != nullptr) {
			if (Projectile->GetIsFrost()) {
				SetSlowedSpeed();
				GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AShip::SetNormalSpeed, 2, false, 2);
			}
			Health = Health - DamageAmount;
			CalculateDead();
		}
	}

	return 0.0f;
}

float AShip::GetFireRate()
{
	return GunComponent->FireRate;
}

void AShip::SetShootingSpeed()
{
	Speed = 0.6f;
}

void AShip::SetNormalSpeed()
{
	Speed = 1.3f;
}

void AShip::SetSlowedSpeed()
{
	Speed = 0.8f;
}

void AShip::ChangeWeapon()
{
	switch (GunComponent->Weapon) {
		case WeaponType::Rapid:
			GunComponent->Initialize(WeaponType::Radial);
			break;

		case WeaponType::Radial:
			GunComponent->Initialize(WeaponType::Frost);
			break;

		case WeaponType::Frost:
			GunComponent->Initialize(WeaponType::Rapid);
			break;

		default:
			break;
	}
}
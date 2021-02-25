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

	NormalSpeed = Speed;

	if (bIsPlayer) {
		Tags.Add(FName("Player"));
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
	FVector ShipLocation = GetActorLocation();
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(ShipLocation, FVector(0, 0, 0));

	if (Health <= 0) {
		Destroy();
		if (!bIsPlayer) {
			if (FMath::RandRange(0, 100) <= 20) {
				if (FMath::RandRange(0, 2) == 0) {
					GetWorld()->SpawnActor<AActor>(HealthDropClass, ShipLocation, SpawnRotation);
				}
				else {
					switch (FMath::RandRange(0, 2)) {

						case 0:
							GetWorld()->SpawnActor<AActor>(RapidWeaponDropClass, ShipLocation, SpawnRotation);
							break;

						case 1:
							GetWorld()->SpawnActor<AActor>(RadialWeaponDropClass, ShipLocation, SpawnRotation);
							break;

						case 2:
							GetWorld()->SpawnActor<AActor>(FrostWeaponDropClass, ShipLocation, SpawnRotation);
							break;

						default:
							break;

					}
				}
			}
		}
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
		if (!OtherActor->ActorHasTag("Projectile") && !OtherActor->ActorHasTag("Enemy") && !OtherActor->ActorHasTag("Drop")) {
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
		if (IsValid(Projectile)) {
			if (Projectile->GetIsFrost()) {
				Slow(Projectile->GetSlowAmount());
				GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AShip::SetNormalSpeed, 2, false, 2);
			}
			Health = Health - DamageAmount;
			CalculateDead();
		}
	}

	return 0.0f;
}

void AShip::AcquireGalaxyPoints(float Points)
{
	GalaxyPoints += Points;
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
	Speed = NormalSpeed;
}

void AShip::Heal(float Amount)
{
	if ((Health + Amount) > 2) {
		Health = 2;
	}
	else {
		Health = Health + Amount;
	}
}

void AShip::Upgrade()
{
	if (Level < 2) {
		Level++;
	}
	GunComponent->Upgrade();
}

void AShip::AcquireWeaponDrop(WeaponType Weapon)
{
	if (GunComponent->Weapon == Weapon) {
		AcquireGalaxyPoints(50);
	}
	else {
		GunComponent->Initialize(Weapon);
	}
}

void AShip::PurchaseUpgrade()
{
	if (GalaxyPoints >= 200) {
		GalaxyPoints -= 200;
		Upgrade();
	}
}

void AShip::Slow(float Amount) 
{
	Speed = Speed * Amount;
}
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

	GunComponent->SetWeapon(WeaponType::Rapid);
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

	if (Angle < 0) {
		Angle = 360;
	}
	else if(Angle > 360){
		Angle = 0;
	}

	if (!bIsPlayer) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FVector2D ScreenLocation;
		PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenLocation);
		if (GEngine) {
			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			if (ScreenLocation.X > 0 && ScreenLocation.X < ViewportSize.X && 
				ScreenLocation.Y > 0 && ScreenLocation.Y < ViewportSize.Y && IsValid(Indicator)) {
				DestroyIndicator();
			}
			else if (IsValid(Indicator)) {
				UpdateIndicator();
			}
			else {
				SpawnIndicator();
			}
		}
	}
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
		if (!bIsPlayer) {
			APopUpMessage* Message = GetWorld()->SpawnActor<APopUpMessage>(PopUpMessageClass, 
																			GetActorLocation(), 
																			FRotator(180,0,180));
			Message->SetTexts("+" + FString::FromInt(Score), "+" + FString::FromInt(GalaxyPoints));
			Message->SetColor(true, Message->ScoreColor);
			Message->SetColor(false, Message->GPColor);

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
		PlayDestroySound();
		ShowExplosion(ShipLocation);
		Destroy();
	}
}

void AShip::CalculateMovement(float AxisValue, float DeltaTime)
{
	Angle = Angle - (AxisValue * Speed * DeltaTime);

	FVector ActorLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
	if (FoundPlanets.Num() > 0) {
		APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);
		if (IsValid(Planet)) {
			ActorLocation.X = sin(Angle * (PI / 180)) * Planet->Radius;
			ActorLocation.Y = cos(Angle * (PI / 180)) * Planet->Radius;
		}
	}

	ActorRotation.Yaw = 90 - Angle;

	SetActorLocation(ActorLocation);
	SetActorRotation(ActorRotation);
}

void AShip::Fire()
{
	if (bIsPlayer) {
		if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle) && GetFireRate() > 0) {
			SetShootingSpeed();
			GunComponent->Fire();
			bCanShoot = false;
			GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AShip::Reload, GetFireRate(), false, GetFireRate());
		}
	}
	else {
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(EnemyProjectileClass,
			GetActorLocation() + (GetActorForwardVector() * 170),
			GetActorRotation());

		if (Projectile) {
			Projectile->SetParameters(GetController(), 0);
		}
	}
}

void AShip::Reload()
{
	bCanShoot = true;
}

WeaponType AShip::GetWeaponType()
{
	return GunComponent->Weapon;
}

void AShip::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bIsPlayer) {
		if (!OtherActor->ActorHasTag("Projectile") && !OtherActor->ActorHasTag("Enemy") && 
			!OtherActor->ActorHasTag("Drop") && !OtherActor->ActorHasTag("Projection")) {
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
	PlayDamageTakenSound();
	if (bIsPlayer) 
	{
		Health = Health - DamageAmount;
		CalculateDead();
	}
	else
	{
		AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
		if (IsValid(Projectile)) {
			if (Projectile->bIsFrost) {
				Slow(Projectile->SlowAmount);
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
	Speed = 60.0f;
}

void AShip::SetNormalSpeed()
{
	Speed = NormalSpeed;
}

bool AShip::AcquireHealthDrop(int DropHealth)
{
	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
		
	APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);
	if (IsValid(Planet)) {
		if (Planet->Health < 3) {
			Planet->Heal(DropHealth);
			return true;
		}
	}
	return false;

}

bool AShip::AcquireWeaponDrop(WeaponType Weapon)
{
	if (GunComponent->Weapon == Weapon) {
		return false;
	}
	else {
		GunComponent->SetWeapon(Weapon);
		ChangeMaterial(Weapon);
	}
	return true;
}

bool AShip::Upgrade()
{
	if (Level < 2) {
		Level++;
		GunComponent->Upgrade();
		return true;
	}
	return false;
}

void AShip::Slow(float Amount) 
{
	SlowedSpeed = NormalSpeed * Amount;
	Speed = SlowedSpeed;
}

FVector AShip::CalculateIndicatorLocation() 
{
	FVector ShipLocation = GetActorLocation();

	if (GEngine) {
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

		float Slope = ShipLocation.X / ShipLocation.Y;

		float TopBorderY = ViewportSize.Y * 1.5;
		float BotBorderY = -ViewportSize.Y * 1.5;
		float RightBorderX = ViewportSize.X * 1.5;
		float LeftBorderX = -ViewportSize.X * 1.5;

		float X = 0;
		float Y = 0;

		if (Angle <= 90) {
			X = TopBorderY / Slope;
			Y = TopBorderY;
			if (X > RightBorderX) {
				Y = RightBorderX * Slope;
				X = RightBorderX;
			}
		}
		else if (Angle > 90 && Angle <= 180) {
			X = TopBorderY / Slope;
			Y = TopBorderY;
			if (X < LeftBorderX) {
				Y = LeftBorderX * Slope;
				X = LeftBorderX;
			}
		}
		else if (Angle > 180 && Angle <= 270) {
			X = BotBorderY / Slope;
			Y = BotBorderY;
			if (X < LeftBorderX) {
				Y = LeftBorderX * Slope;
				X = LeftBorderX;
			}
		}
		else {
			X = BotBorderY / Slope;
			Y = BotBorderY;
			if (X > RightBorderX) {
				Y = RightBorderX * Slope;
				X = RightBorderX;
			}
		}
		return FVector(Y, X, 0);
	}
	return FVector(0, 0, 0);
}

void AShip::SpawnIndicator()
{
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), GetActorLocation());
	Indicator = GetWorld()->SpawnActor<AIndicator>(IndicatorClass, CalculateIndicatorLocation(), SpawnRotation);
	if (IsValid(Indicator)) {
		Indicator->Distance = FVector::Dist(Indicator->GetActorLocation(), GetActorLocation());
	}
}

void AShip::UpdateIndicator()
{
	if (IsValid(Indicator)) {
		Indicator->SetActorLocation(CalculateIndicatorLocation());
		Indicator->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Indicator->GetActorLocation(), GetActorLocation()));
		Indicator->Distance = FVector::Dist(Indicator->GetActorLocation(), GetActorLocation());
	}
}

void AShip::DestroyIndicator()
{
	if (IsValid(Indicator)) {
		Indicator->Destroy();
	}
	Indicator = nullptr;
}

void AShip::SpaceTruckExplode(float SpaceTruckAngle)
{
	for (int i = 0; i < 3; ++i) {
		AShip* SpawnedParticle = GetWorld()->SpawnActor<AShip>(SpaceTruckParticleClass, 
																GetActorLocation(), 
																GetActorRotation());

		if (IsValid(SpawnedParticle)) {
			SpawnedParticle->Initialize(SpaceTruckAngle);
		}
	}
}

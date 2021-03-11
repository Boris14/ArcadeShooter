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

	bShouldShowBonusGP = false;
	bShouldShowBonusScore = false;
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
	SpawnIndicator();
}

void AShip::CalculateDead()
{
	FVector ShipLocation = GetActorLocation();
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(ShipLocation, FVector(0, 0, 0));

	if (Health <= 0) {
		PlayDestroySound();
		Destroy();
		if (IsValid(Indicator)) {
			DestroyIndicator();
		}
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

	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);

	APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);

	if (IsValid(Planet)) {
		ActorLocation.X = sin(Angle * (PI / 180)) * Planet->Diameter;
		ActorLocation.Y = cos(Angle * (PI / 180)) * Planet->Diameter;
	}

	ActorRotation.Yaw = 90 - Angle;

	SetActorLocation(ActorLocation);
	SetActorRotation(ActorRotation);
}

void AShip::Fire()
{
	bCanShoot = false;
	GunComponent->Fire();
	if (!GetWorldTimerManager().IsTimerActive(MemberTimerHandle)) {
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AShip::Reload, GetFireRate(), false, GetFireRate());
	}
}

void AShip::Reload()
{
	bCanShoot = true;
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
	PlayDamageTakenSound();

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
	Speed = NormalSpeed;
}

void AShip::AcquireHealthDrop(int DropHealth)
{
	bShouldShowBonusScore = false;

	TArray<AActor*> FoundPlanets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
		
	APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);
	if (IsValid(Planet)) {
		if (Planet->Health < 3) {
			Planet->Heal(DropHealth);
		}
		else {
			bShouldShowBonusScore = true;
		}
	}

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

void AShip::AcquireWeaponDrop(WeaponType Weapon)
{
	if (GunComponent->Weapon == Weapon) {
		bShouldShowBonusGP = true;
	}
	else {
		GunComponent->Initialize(Weapon);
		ChangeMaterial(Weapon);
	}
}

void AShip::Slow(float Amount) 
{
	Speed = Speed * Amount;
}

FVector AShip::CalculateIndicatorLocation() 
{
	FVector ShipLocation = GetActorLocation();

	if (GEngine) {
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

		float Slope = ShipLocation.X / ShipLocation.Y;

		float TopBorderY = ViewportSize.Y * 2.3;
		float BotBorderY = -ViewportSize.Y * 2.3;
		float RightBorderX = ViewportSize.X * 2.2;
		float LeftBorderX = -ViewportSize.X * 2.2;

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
	Indicator = Cast<AIndicator>(GetWorld()->SpawnActor<AActor>(IndicatorClass, CalculateIndicatorLocation(), SpawnRotation));
	if (IsValid(Indicator)) {
		Indicator->TargetShip = this;
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
	Indicator->Destroy();
	Indicator = nullptr;
}

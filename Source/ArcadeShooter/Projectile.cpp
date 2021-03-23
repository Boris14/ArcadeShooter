// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add(FName("Projectile"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(2);
}

void AProjectile::SetParameters(AController* InitController, int ProjectileLevel)
{
	FVector Velocity = FVector(Speed, 0, 0);
	ProjectileMovementComponent->SetVelocityInLocalSpace(Velocity);

	ShooterController = InitController;

	Level = ProjectileLevel;
}

void AProjectile::Initialize()
{
	const FString Context(TEXT("WeaponLevels"));
	for (int i = 1; i <= 3; ++i) {
		FString RowIndex = "Level" + FString::FromInt(i);
		WeaponLevels.Add(WeaponData->FindRow<FWeaponStruct>(FName(RowIndex), Context, true));
	}

	if (bIsFrost) {
		SlowAmount = WeaponLevels[Level]->FrostSlowing;
	}

	if (bIsRadial) {
		Damage = WeaponLevels[Level]->RadialProjectileDamage;
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponLevels.Num() < 1 && IsValid(WeaponData)) {
		Initialize();
	}
}


float AProjectile::GetDamage()
{
	return Damage;
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	FDamageEvent DamageEvent;

	if (!((OtherActor->ActorHasTag("Player") && !bIsFromEnemy) || (OtherActor->ActorHasTag("Enemy") && bIsFromEnemy))) {
		OtherActor->TakeDamage(Damage, DamageEvent, ShooterController, this);
	}

	if (!bIsRadial && !(OtherActor->ActorHasTag("Player") && !bIsFromEnemy) && 
		!(OtherActor->ActorHasTag("Enemy") && bIsFromEnemy) && 
		!OtherActor->ActorHasTag("Drop") && !OtherActor->ActorHasTag("Projectile") &&
		!OtherActor->ActorHasTag("Projection")) {
		Destroy();
	}
	else if (OtherActor->ActorHasTag("Projectile") && bIsFromEnemy) {
		Destroy();
	}

}

float AProjectile::GetSlowAmount()
{
	return SlowAmount;
}

bool AProjectile::GetIsFrost()
{
	return bIsFrost;
}

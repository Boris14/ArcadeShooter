// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add(FName("Projectile"));

	/*CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	CollisionComponent->InitSphereRadius(10.0f);
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;

	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	SphereVisual->SetupAttachment(RootComponent);
	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SphereVisual->SetWorldScale3D(FVector(1.0f));
	}*/

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2);
	
}

void AProjectile::Initialize(AController* InitController)
{
	FVector Velocity = FVector(Speed, 0, 0);
	ProjectileMovementComponent->SetVelocityInLocalSpace(Velocity);

	ShooterController = InitController;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	if (!bIsRadial && !(OtherActor->ActorHasTag("Player") && !bIsFromEnemy) && !(OtherActor->ActorHasTag("Enemy") && bIsFromEnemy) && !OtherActor->ActorHasTag("Drop")) {
		Destroy();
	}

}

bool AProjectile::GetIsFrost()
{
	return bIsFrost;
}

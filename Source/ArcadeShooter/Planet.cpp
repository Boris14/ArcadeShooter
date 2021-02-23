// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"

// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 3;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	MeshComponent->SetupAttachment(RootComponent);

	Tags.Add(FName("Planet"));
}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Yaw = ActorRotation.Yaw + (DeltaTime * 5);

	SetActorRotation(ActorRotation);


}

void APlanet::CalculateDead()
{
	if (Health <= 0) {
		Destroy();
	}
}

void APlanet::Heal(float Amount)
{
	if (Health <= 2) {
		Health = Health + Amount;
	}
}

float APlanet::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health = Health - DamageAmount;
	CalculateDead();
	return 0.0f;
}



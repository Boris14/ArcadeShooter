// Fill out your copyright notice in the Description page of Project Settings.


#include "Indicator.h"

// Sets default values
AIndicator::AIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	Brightness = 0;
	Distance = 0;
}

// Called when the game starts or when spawned
void AIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Brightness = 2.5 / FMath::Square(Distance/1000);

	if (Brightness > 3) {
		Brightness = 3;
	}
}


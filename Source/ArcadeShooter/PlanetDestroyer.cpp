// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetDestroyer.h"

APlanetDestroyer::APlanetDestroyer()
{
	MeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent2"));
	MeshComponent2->SetupAttachment(RootComponent);

	MeshComponent3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent3"));
	MeshComponent3->SetupAttachment(RootComponent);
}

void APlanetDestroyer::BeginPlay()
{
	Super::BeginPlay();

	if (bIsBody) {
		MaxHealth = Health;
	}

}

void APlanetDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}



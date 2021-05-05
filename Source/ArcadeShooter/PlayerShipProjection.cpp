// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipProjection.h"

// Sets default values
APlayerShipProjection::APlayerShipProjection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add(FName("Projection"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerShipProjection::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(3);
}

// Called every frame
void APlayerShipProjection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerShipProjection::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		bIsOverlapping = true;
	}
}

void APlayerShipProjection::NotifyActorEndOverlap(AActor* OtherActor)
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerShipProjection::EndOverlapping, 0.2, false, 0.2);
}

void APlayerShipProjection::EndOverlapping()
{
	bIsOverlapping = false;
}


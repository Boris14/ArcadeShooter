// Fill out your copyright notice in the Description page of Project Settings.


#include "Drop.h"

// Sets default values
ADrop::ADrop()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	Tags.Add(FName("Drop"));
}

// Called when the game starts or when spawned
void ADrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrop::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {

		AShip* PlayerShip = Cast<AShip>(OtherActor);

		if (IsValid(PlayerShip)) {
			if (bIsHealth) {
				if (PlayerShip->Health >= 100) {

					TArray<AActor*> FoundPlanets;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundPlanets);
					APlanet* Planet = Cast<APlanet>(FoundPlanets[0]);
					if (IsValid(Planet)) {
						Planet->Heal(HealAmount);
					}

				}
				else {
					PlayerShip->Heal(HealAmount);
				}
			}
			else {
				PlayerShip->ChangeWeapon();
			}
		}
		Destroy();
	}
	else if (OtherActor->ActorHasTag("Planet")) {
		Destroy();
	}
}


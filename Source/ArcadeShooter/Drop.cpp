// Fill out your copyright notice in the Description page of Project Settings.


#include "Drop.h"
///
// Sets default values
ADrop::ADrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * 2));

}

void ADrop::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {

		AShip* PlayerShip = Cast<AShip>(OtherActor);

		if (IsValid(PlayerShip)) {
			if (bIsHealth) {
				PlayerShip->AcquireHealthDrop(HealAmount);
			}
			else {
				PlayerShip->AcquireWeaponDrop(WeaponDropType);
			}
		}
		ShowPopUpMessage();
		Destroy();
	}
	else if (OtherActor->ActorHasTag("Planet")) {
		Destroy();
	}
}

bool ADrop::FindBonuses(bool bForScore)
{
	AArcadeShooterGameModeBase* GameMode = Cast<AArcadeShooterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode)) {
		for (AShip* Ship : GameMode->PlayerShips) {
			if (IsValid(Ship)) {
				if (bForScore && Ship->bShouldShowBonusScore) {
					Ship->bShouldShowBonusScore = false;
					return true;
				}
				else if (Ship->bShouldShowBonusGP) {
					Ship->bShouldShowBonusGP = false;
					return true;
				}
			}
		}
	}
	return false;
}

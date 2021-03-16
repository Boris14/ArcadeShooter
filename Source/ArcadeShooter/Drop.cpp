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

	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));

}

void ADrop::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {

		AShip* PlayerShip = Cast<AShip>(OtherActor);

		if (IsValid(PlayerShip)) {
			if (bIsHealth) {
				if (!PlayerShip->AcquireHealthDrop(HealAmount)) {
					ShowPopUpMessage(true);
					IncrementScore(ScoreAmount);
				}
				else {
					ShowPopUpMessage(false);
				}
			}
			else if(!PlayerShip->AcquireWeaponDrop(WeaponDropType)){
				IncrementGalaxyPoints(GalaxyPointsAmount);
				ShowPopUpMessage(true);
			}
			else {
				ShowPopUpMessage(false);
			}
		}
		PlayPickUpSound();
		Destroy();
	}
	else if (OtherActor->ActorHasTag("Planet")) {
		Destroy();
	}
}

void ADrop::ShowPopUpMessage(bool bIsForBonus)
{
	APopUpMessage* Message = GetWorld()->SpawnActor<APopUpMessage>(PopUpMessageClass,
		GetActorLocation(),
		FRotator(180, 0, 180));
	if (bIsHealth) {
		if (bIsForBonus) {
			Message->SetTexts("+500", "");
			Message->SetColor(true, Message->ScoreColor);
		}
		else {
			Message->SetTexts("+1", "");
			Message->SetColor(true, Message->HPColor);
		}
	}
	else {
		if (bIsForBonus) {
			Message->SetTexts("+50", "");
			Message->SetColor(true, Message->GPColor);
		}
	}
}
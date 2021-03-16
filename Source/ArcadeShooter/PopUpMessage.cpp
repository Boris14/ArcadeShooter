// Fill out your copyright notice in the Description page of Project Settings.


#include "PopUpMessage.h"

// Sets default values
APopUpMessage::APopUpMessage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));

	PrimaryText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PrimaryText"));
	SecondaryText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SecondaryText"));

	PrimaryText->SetupAttachment(RootComponent);
	SecondaryText->SetupAttachment(PrimaryText);

	PrimaryText->SetText(FText::AsCultureInvariant(""));
	SecondaryText->SetText(FText::AsCultureInvariant(""));
}

// Called when the game starts or when spawned
void APopUpMessage::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(2);
}

// Called every frame
void APopUpMessage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APopUpMessage::SetTexts(FString Primary, FString Secondary)
{
	PrimaryText->SetText(FText::AsCultureInvariant(Primary));
	SecondaryText->SetText(FText::AsCultureInvariant(Secondary));
}

void APopUpMessage::SetColor(bool bForPrimaryText, FColor ColorValue)
{
	if (bForPrimaryText) {
		PrimaryText->SetTextRenderColor(ColorValue);
	}
	else {
		SecondaryText->SetTextRenderColor(ColorValue);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Math/Color.h"
#include "PopUpMessage.generated.h"

UCLASS(Blueprintable)
class ARCADESHOOTER_API APopUpMessage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APopUpMessage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTexts(FString Primary, FString Secondary);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextRenderComponent* PrimaryText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextRenderComponent* SecondaryText;

	void SetColor(bool bForPrimaryText, FColor ColorValue);

	FColor ScoreColor = FColor(0, 230, 255, 1);

	FColor GPColor = FColor(255, 227, 20, 1);
	
	FColor HPColor = FColor(0, 255, 38, 1);
};

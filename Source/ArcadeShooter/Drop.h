// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Planet.h"
#include "Gun.h"
#include "GameFramework/Actor.h"
#include "Drop.generated.h"

UCLASS()
class ARCADESHOOTER_API ADrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHealth = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<WeaponType> WeaponDropType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HealAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScoreAmount = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GalaxyPointsAmount = 50;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APopUpMessage> PopUpMessageClass;

	void ShowPopUpMessage(bool bIsForBonus);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void IncrementScore(int Amount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void IncrementGalaxyPoints(int Amount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayPickUpSound();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

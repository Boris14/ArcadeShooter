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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHealth = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<WeaponType> WeaponDropType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount = 1;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "PlanetDestroyer.generated.h"

/**
 * 
 */
UCLASS()
class ARCADESHOOTER_API APlanetDestroyer : public AShip
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APlanetDestroyer();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBody;

	float MaxHealth = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Planet.generated.h"

UCLASS(Blueprintable)
class ARCADESHOOTER_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CalculateDead();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	void Heal(int Amount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayTakenDamageSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Diameter;

	virtual float TakeDamage(float DamageAmount,
							FDamageEvent const& DamageEvent,
							AController* EventInstigator,
							AActor* DamageCauser) override;
};

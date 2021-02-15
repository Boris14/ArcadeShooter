// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTags/Classes/GameplayTagsManager.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "GameplayTagsModule.h"
#include "GameplayTagsSettings.h"
#include "GameplayTags.h"
#include "GameplayTagsManager.h"
#include "Projectile.generated.h"

UCLASS(Blueprintable)
class ARCADESHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(AController* InitController);

	AController* ShooterController;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent *ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFromEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRadial = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFrost = false;

	float GetDamage();

	bool GetIsFrost();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

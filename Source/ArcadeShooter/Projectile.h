// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "WeaponStruct.h"
#include "PopUpMessage.h"
#include "Kismet/GameplayStatics.h"
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

	void SetParameters(AController* InitController, int ProjectileLevel);

	void Initialize();

	AController* ShooterController;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent *ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SlowAmount = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponData;

	TArray<FWeaponStruct*> WeaponLevels;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APopUpMessage> PopUpMessageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFromEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRadial = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFrost = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BonusGalaxyPoints = 20;

	UFUNCTION(BlueprintImplementableEvent)
	void IncrementGalaxyPoints();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayReflectSound();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

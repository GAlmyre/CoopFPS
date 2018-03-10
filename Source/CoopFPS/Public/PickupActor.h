// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class APowerUpActor;

UCLASS()
class COOPFPS_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn();

	UPROPERTY(VisibleAnywhere, Category ="Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<APowerUpActor> PowerUpClass;

	APowerUpActor* PowerUpInstance;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
	float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;
};

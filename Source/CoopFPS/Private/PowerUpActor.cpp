// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUpActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APowerUpActor::APowerUpActor()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0.0f;

	bIsPowerUpActive = false;

	SetReplicates(true);
}

void APowerUpActor::OnTickPowerUp()
{
	TicksProcessed++;

	OnPowerUpTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpired();

		bIsPowerUpActive = false;

		OnRep_PowerUpActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTick);
	}
}


void APowerUpActor::OnRep_PowerUpActive()
{
	OnPowerUpStateChanged(bIsPowerUpActive);
}

void APowerUpActor::ActivatePowerUp(AActor* OtherActor)
{
	OnActivated(OtherActor);

	bIsPowerUpActive = true;
	OnRep_PowerUpActive();

	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTick, this, &APowerUpActor::OnTickPowerUp, PowerupInterval, true);
	}
	else
	{
		OnTickPowerUp();
	}
}

void APowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerUpActor, bIsPowerUpActive);
}

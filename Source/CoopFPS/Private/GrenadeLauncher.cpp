// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadeLauncher.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

void AGrenadeLauncher::Fire()
{
	AActor* Owner = GetOwner();
	if (ProjectileClass && Owner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = GetActorRotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ActorSpawnParams.Instigator = Instigator;

		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);

		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		}
	}
}

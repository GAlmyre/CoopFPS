// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GrenadeLauncher.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class COOPFPS_API AGrenadeLauncher : public AWeapon
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AProjectile> ProjectileClass;
	
public:

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire() override;
};

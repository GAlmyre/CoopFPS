// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Weapon.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionProfileName("Projectile");	// set up a notification for when this component hits something blocking
																			
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	InitialLifeSpan = 1.f;
	// start a timer then wait for destruction
	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &AProjectile::Explode, InitialLifeSpan);
}

void AProjectile::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("EXPLODE"));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

    UGameplayStatics::ApplyRadialDamage(this, 30.0f, GetActorLocation(), 300.0f, DamageType, IgnoredActors, nullptr, nullptr, true);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 300.0f, 32, FColor::Orange, false, 1.0f, 0, 1.0f);
	if (ImpactEffet)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffet,GetActorLocation(), FRotator(0), FVector(3));
	}
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HordeGameMode.generated.h"

enum class EWaveState : uint8;

/**
 * 
 */
UCLASS()
class COOPFPS_API AHordeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	AHordeGameMode();

protected:

	// Hook for BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent, Category ="GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	// Start spawning
	void StartWave();

	// Stop spawning
	void EndWave();

	// Set timer for next wave
	void PrepareForNextWave();

	// Check if all the bots are dead
	void CheckWaveState();

	// Handles GameOver
	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	// bots to spawn in current wave
	int32 NumberOfBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;
};

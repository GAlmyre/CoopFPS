// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeGameMode.h"
#include "TimerManager.h"
#include "Components/HealthComponent.h"
#include "HordeGameState.h"

AHordeGameMode::AHordeGameMode()
{
	TimeBetweenWaves = 2.0f;

	GameStateClass = AHordeGameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AHordeGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AHordeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

void AHordeGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AHordeGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void AHordeGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawn--;

	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void AHordeGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);

}

void AHordeGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AHordeGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);
}

void AHordeGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	// we have still bots to spawn
	if (NumberOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	// Check for alive, non player controlled pawns
	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		UHealthComponent* HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));

		if (HealthComp && HealthComp->GetHealth())
		{
			bIsAnyBotAlive = true;
			break;
		}
	}
	
	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void AHordeGameMode::CheckAnyPlayerAlive()
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PlayerController = It->Get();

		if (PlayerController && PlayerController->GetPawn())
		{
			APawn* Pawn = PlayerController->GetPawn();
			UHealthComponent* HealtComponent = Cast<UHealthComponent>(Pawn->GetComponentByClass(UHealthComponent::StaticClass()));

			if (ensure(HealtComponent) && HealtComponent->GetHealth() > 0.0f)
			{
				// a player is still alive
				return;
			}

		}
	}
	
	// No player alive
	GameOver();
}

void AHordeGameMode::GameOver()
{
	EndWave();

	// @TODO : finish match and present GameOver screen
	UE_LOG(LogTemp, Log, TEXT("GAME OVER! Players died"));

	SetWaveState(EWaveState::GameOver);
}

void AHordeGameMode::SetWaveState(EWaveState NewState)
{
	AHordeGameState* GameState = GetGameState<AHordeGameState>();

	if (ensureAlways(GameState))
	{
		GameState->SetWaveState(NewState);
	}
}

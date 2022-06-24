// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RestartPacmanGameModeBase.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class EGameState :uint8
{
	EMenu,
	EPlay,
	EPause,
	EWin,
	EGameOver
};

UCLASS()
class RESTARTPACMAN_API ARestartPacmanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARestartPacmanGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PacdotNums;

	void StartGame();
	void PauseGame();
	void RestartGame();
	void ExitGame();
	void SetEnemiesVulnerable();

	void RecordEnemyTimer();
	void RestoreEnemyTimer();
	

	EGameState GetCurrentState() const;
	void SetCurrentState(EGameState State);

	int GetPacdotNums() const;
	void SetPacdotNums(int nums);


private:
	TArray<class AGameEnemy*> Enemies; 
	TArray<float> EnemiesTimerRemaining;
};

FORCEINLINE EGameState ARestartPacmanGameModeBase::GetCurrentState() const
{
	return CurrentState;
}

FORCEINLINE int ARestartPacmanGameModeBase::GetPacdotNums() const
{
	return PacdotNums;
}
// Copyright Epic Games, Inc. All Rights Reserved.


#include "RestartPacmanGameModeBase.h"
#include "GameEnemy.h"
#include "GamePlayer.h"
#include "EnemyAIController.h"
#include "Engine/Public/EngineUtils.h"
#include "Pacdot.h"


ARestartPacmanGameModeBase::ARestartPacmanGameModeBase()
{
	CurrentState = EGameState::EMenu;
}

void ARestartPacmanGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AGameEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
	{
		Enemies.Add(Cast<AGameEnemy>(*EnemyItr));
	}

	for (TActorIterator<APacdot> PacItr(GetWorld()); PacItr; ++PacItr)
	{
		PacdotNums++;
	}
}

void ARestartPacmanGameModeBase::StartGame()
{
	if (CurrentState == EGameState::EMenu)
	{
		SetCurrentState(EGameState::EPlay);

		//开始游戏后，让所有敌人开始由AI controller进行控制移动
		for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
		{
			Cast<AEnemyAIController>((*Iter)->GetController())->GotoNewDestination();
		}
	}
}

void ARestartPacmanGameModeBase::PauseGame()
{
	if (CurrentState == EGameState::EPlay)
	{
		SetCurrentState(EGameState::EPause);
	}
	else if (CurrentState == EGameState::EPause)
	{
		SetCurrentState(EGameState::EPlay);
	}
}

void ARestartPacmanGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("RestartLevel");
}

void ARestartPacmanGameModeBase::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}

void ARestartPacmanGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		(*Iter)->SetVulnerable();
	}
}

void ARestartPacmanGameModeBase::RecordEnemyTimer()
{
	EnemiesTimerRemaining.Empty();
	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		if ((*Iter)->IsVulnerable == false && (*Iter)->IsDead == false)
		{
			EnemiesTimerRemaining.Add(-1);
		}
		if ((*Iter)->IsVulnerable == true && (*Iter)->IsDead == false)
		{
			float VulnerableRemainingTime = GetWorldTimerManager().GetTimerRemaining((*Iter)->VulnerableTimerHandle);
			EnemiesTimerRemaining.Add(VulnerableRemainingTime);
		}
		if ((*Iter)->IsDead == true)
		{
			float DeadRemainingTime = GetWorldTimerManager().GetTimerRemaining((*Iter)->DeadTimerHandle);
			EnemiesTimerRemaining.Add(DeadRemainingTime);
		}
	}
}

void ARestartPacmanGameModeBase::RestoreEnemyTimer()
{
	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		if ((*Iter)->IsVulnerable == false && (*Iter)->IsDead == false)
		{
			(*Iter)->SetVulnerable(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
		if ((*Iter)->IsDead == true)
		{
			(*Iter)->SetDead(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
	}
}

 

void ARestartPacmanGameModeBase::SetCurrentState(EGameState State)
{
	CurrentState = State;
	switch (CurrentState)
	{
	case EGameState::EMenu:
		UE_LOG(LogTemp, Warning, TEXT("Menu"));
		break;
	case EGameState::EPlay:
		UE_LOG(LogTemp, Warning, TEXT("Play"));
		for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
		{
			(*Iter)->SetMovement(true);
		}
		Cast<AGamePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);//获取player
		break;
	case EGameState::EPause:
		UE_LOG(LogTemp, Warning, TEXT("Pause"));
		for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
		{
			(*Iter)->SetMovement(false);
		}
		Cast<AGamePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);
		break;
	case EGameState::EWin:
		UE_LOG(LogTemp, Warning, TEXT("Win"));
		for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
		{
			(*Iter)->Destroy();
		}
		break;
	case EGameState::EGameOver:
		UE_LOG(LogTemp, Warning, TEXT("Gameover"));
		for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
		{
			(*Iter)->SetMovement(false);
		}
		Cast<AGamePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);
		break;
	default:
		break;
	}
}

void ARestartPacmanGameModeBase::SetPacdotNums(int nums)
{
	PacdotNums = nums;
}

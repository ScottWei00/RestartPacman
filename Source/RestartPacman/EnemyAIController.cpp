// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "GameEnemy.h"
#include "NavigationSystem.h"

void AEnemyAIController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	ControlledEnemy = Cast<AGameEnemy>(PawnToPossess);//AI¿ØÖÆÆ÷È¥¿ØÖÆenemy
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	GotoNewDestination();
}

void AEnemyAIController::GotoNewDestination()
{
	UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(this);
	if (NavMesh)
	{
		const float SearchRadius = 10000.f;
		FNavLocation RandomLocation;
		const bool bNewDestinationFound = NavMesh->GetRandomReachablePointInRadius(
			ControlledEnemy->GetActorLocation(), SearchRadius, RandomLocation);
		
		if (bNewDestinationFound)
		{
			MoveToLocation(RandomLocation.Location, 50.f);
		}
	}
}

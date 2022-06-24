// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GamePlayer.h"
#include "Engine/Public/TimerManager.h"


// Sets default values
AGameEnemy::AGameEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyDisplayMesh"));
	EnemyDisplayMesh->SetupAttachment(GetCapsuleComponent());

	IsDead = false;
	IsVulnerable = false;

	NormalSpeed = 600.0f;
	VulnerableSpeed = 100.0f;

	VulnerableTime = 3.0f;
	DeadTime = 3.0f;



	

}

// Called when the game starts or when spawned
void AGameEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetMovement(true);
	StartLocation = GetActorLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGameEnemy::OnEmemyBeginOverlap);
}
// Called every frame
void AGameEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameEnemy::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		if (IsVulnerable == false && IsDead == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
		if (IsVulnerable == true && IsDead == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = VulnerableSpeed;
		}
		if (IsDead == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;
		}
	} 
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void AGameEnemy::SetNormal()
{
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);
	GetWorldTimerManager().ClearTimer(DeadTimerHandle);

	IsVulnerable = false;
	IsDead = false;
	SetMovement(true);
}

void AGameEnemy::SetDead(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer&&IsDead)
	{
		GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AGameEnemy::SetNormal, Time, false);
		return;
	} 
	if (IsDead)
	{
		return;
	}
	IsDead = true;
	SetMovement(true);
	SetActorLocation(StartLocation);
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AGameEnemy::SetNormal, DeadTime, false);
}

void AGameEnemy::SetVulnerable(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer)
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &AGameEnemy::SetNormal, Time, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &AGameEnemy::SetNormal, VulnerableTime, false);
	}
	if (IsVulnerable)
	{
		return;
	}
	
	IsVulnerable = true;
	SetMovement(true);
}

void AGameEnemy::OnEmemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AGamePlayer::StaticClass()))
	{
		if (IsDead)
		{
			return;
		}
		if (IsVulnerable)
		{
			SetDead();
		}
		else
		{
			Cast<AGamePlayer>(OtherActor)->Injured();
		}
	}
}


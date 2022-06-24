// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "RestartPacmanGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pacdot.h"
#include "Engine/Public/TimerManager.h"


// Sets default values
AGamePlayer::AGamePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Velocity = FVector(0, 0, 0);

	NormalSpeed = 600.0f;
	Lifes = 3;
	invicibleTime = 3.0f;
	

	PlayerDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerDisplayMesh"));
	PlayerDisplayMesh->SetupAttachment(GetCapsuleComponent());
}



// Called when the game starts or when spawned
void AGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	GameModeRef = Cast<ARestartPacmanGameModeBase>(UGameplayStatics::GetGameMode(this));

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGamePlayer::OnPlayerBeginOverlay);
	SetMovement(true);
}

// Called every frame
void AGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &AGamePlayer::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &AGamePlayer::MoveY);

	PlayerInputComponent->BindAction("StartGame", IE_Pressed, this, &AGamePlayer::StartGame);
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &AGamePlayer::PauseGame);
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &AGamePlayer::RestartGame);
	PlayerInputComponent->BindAction("ExitGame", IE_Pressed, this, &AGamePlayer::ExitGame);

}

void AGamePlayer::MoveX(float Value)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
	{
		Velocity.X = Value;
		Velocity.Y = 0;

		if (Value > 0)
		{
			SetActorRotation(FRotator(0, 0, 0));
		}
		else if (Value < 0)
		{
			SetActorRotation(FRotator(0, -180, 0));
		}
		AddMovementInput(Velocity);
	}
}

void AGamePlayer::MoveY(float Value)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
	{
		Velocity.X = 0;
		Velocity.Y = Value;

		if (Value > 0)
		{
			SetActorRotation(FRotator(0, 90, 0));
		}
		else if (Value < 0)
		{
			SetActorRotation(FRotator(0, -90, 0));
		}

		AddMovementInput(Velocity);
	}
}

void AGamePlayer::StartGame()
{
	GameModeRef->StartGame();
}

void AGamePlayer::PauseGame()
{
	GameModeRef->PauseGame();
}

void AGamePlayer::RestartGame()
{
	GameModeRef->RestartGame();
}

void AGamePlayer::ExitGame()
{
	GameModeRef->ExitGame();
}

void AGamePlayer::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void AGamePlayer::Injured()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invicible"));
	Lifes--;
	UE_LOG(LogTemp, Warning, TEXT("Life"));
	if (Lifes == 0)
	{
		GameModeRef->SetCurrentState(EGameState::EGameOver);
	}
	else
	{
		SetActorLocation(StartLocation);
		GetWorldTimerManager().SetTimer(ResettingTimerHandle, this, &AGamePlayer::Resetting, invicibleTime, false);
	}
}

void AGamePlayer::Resetting()
{
	GetWorldTimerManager().ClearTimer(ResettingTimerHandle);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

void AGamePlayer::OnPlayerBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
	{
		if (OtherActor->IsA(APacdot::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("pacdot"));
			APacdot* CurrentActor = Cast<APacdot>(OtherActor);
			if (CurrentActor->IsSuperPacdot)
			{
				GameModeRef->SetEnemiesVulnerable();
			}
			OtherActor->Destroy();
			GameModeRef->SetPacdotNums(GameModeRef->GetPacdotNums() - 1);
		}
	}
}

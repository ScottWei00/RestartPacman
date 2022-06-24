// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GamePlayer.generated.h"


UCLASS()
class RESTARTPACMAN_API AGamePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGamePlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PlayerDisplayMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Lifes;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveX(float Value);
	void MoveY(float Value);

	void StartGame();
	void PauseGame();
	void RestartGame();
	void ExitGame();

	void SetMovement(bool bCanMove);

	void Injured();
	void Resetting();

	UFUNCTION()
	void OnPlayerBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	FVector Velocity;

	float NormalSpeed;
	float invicibleTime;

	

	FVector StartLocation;
	
	class ARestartPacmanGameModeBase* GameModeRef;

	FTimerHandle ResettingTimerHandle;
};

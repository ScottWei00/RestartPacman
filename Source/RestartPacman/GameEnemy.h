// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameEnemy.generated.h"

UCLASS()
class RESTARTPACMAN_API AGameEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameEnemy();

	//创建敌人的静态组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* EnemyDisplayMesh;

	bool IsDead;
	bool IsVulnerable;

	FTimerHandle VulnerableTimerHandle;
	FTimerHandle DeadTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetMovement(bool bCanMove);
	void SetNormal();
	void SetDead(bool bRestoreTimer = false, float Time = 0.0f);
	void SetVulnerable(bool bRestoreTimer = false, float Time = 0.0f);

	UFUNCTION()
	void OnEmemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	
	FVector StartLocation;


	float VulnerableSpeed;
	float NormalSpeed;
	float DeadTime;
	float VulnerableTime;

	

};

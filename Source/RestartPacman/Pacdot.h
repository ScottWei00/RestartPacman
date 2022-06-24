// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pacdot.generated.h"

UCLASS()
class RESTARTPACMAN_API APacdot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APacdot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* PacdotCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacdotDisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSuperPacdot = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

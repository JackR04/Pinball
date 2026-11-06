// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "PawnMachine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

UCLASS()
class PINBALL_API ABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABumper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* bumperMesh;
	UPROPERTY(EditAnywhere)
	USphereComponent* sphereCollider;
	UPROPERTY(EditAnywhere)
	APawnMachine* machine;
	FVector startSize;
	FVector endSize;
	FVector targetSize;
	void Grow();
	UPROPERTY(EditAnywhere)
	int points;

};

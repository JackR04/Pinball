// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "plunger.generated.h"

UCLASS()
class PINBALL_API Aplunger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aplunger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* plungerMesh;

	FVector startLocation;
	FVector endLocation;
	FVector newLocation;
	bool bPullBackPlunger;
	bool bFirePlunger;
	bool bPlungerIsMoving;
	UPROPERTY(EditAnywhere)
	int plungerLength;
	UPROPERTY(EditAnywhere)
	float pullBackSpeed;

	UPROPERTY(EditAnywhere)
	float firedSpeed;

	void PullBack(float DeltaTime);

	void Fired(float DeltaTime);
};

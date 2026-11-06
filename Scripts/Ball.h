// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include"multiBall.h"
#include "killBox.h"
#include "engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class APawnMachine;
UCLASS()
class PINBALL_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	float gravity;
	FVector velocity;
	UStaticMeshComponent* ballMesh;
	UPROPERTY()
	USphereComponent* sphereCollider;
	FVector machineMiddle;
	void SetMachinePawnMiddle(AActor* machine);
	
	UPROPERTY()
	APawnMachine* pawnMachine;
	
};

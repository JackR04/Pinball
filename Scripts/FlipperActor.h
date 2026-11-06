// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "engine.h"
#include "Ball.h"
#include "FlipperActor.generated.h"



UENUM()
enum class EFlipperSide : uint8
{
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right")
};
UCLASS()
class PINBALL_API AFlipperActor : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AFlipperActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FRotator targetRotation;
	FRotator openRotation;
	FRotator closedRotation;
	TArray<ABall*> ballList;
	float speed = 12.0f;
	//bool bIsPressed;
	bool bIsMoving;
	UPROPERTY(EditAnywhere, Category = "Side")
	EFlipperSide flipperSide = EFlipperSide::LEFT;
	
	UPROPERTY (EditAnywhere)
	UStaticMeshComponent* flipperMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* overlapComponent;

	
	void OnFlipperPress();
	void OnFlipperRelease();

	
};

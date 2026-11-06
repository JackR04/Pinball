// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "engine.h"
#include "Ball.h"
#include "plunger.h"
#include "FlipperActor.h"
#include "lightbulb.h"
#include "scoreHud.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnMachine.generated.h"
UENUM()
enum class EAssistLevel : uint8
{
	OFF,
	ONE,
	TWO,
	THREE
};
UCLASS()
class PINBALL_API APawnMachine : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void FirePressed();
	void FireReleased();
	void NudgeLeft();
	void NudgeRight();
	void LeftFlipperPress();
	void LeftFlipperRelease();
	void RightFlipperPress();
	void RightFlipperRelease();
	void Restart();
	void UpdateScore(int points);
	void GetLights();
	void KillBall(ABall* ball);
	void EndGame();
	void MultiBall();

	TArray<Alightbulb*> storedLights;
	UPROPERTY(EditAnywhere)
	UCameraComponent* camera;
	FRotator targetRotation;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* visualMesh;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* multiBallSpawnPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* pinballSpawnLocation;

	UPROPERTY(EditAnywhere)
	USceneComponent* ballsLeftSpawnPoint;

	TArray<ABall*> ballsLeftArray;

	UPROPERTY(EditAnywhere, Category = "Flippers Array")
	TArray<AFlipperActor*> flippers;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UscoreHud> scoreHud;

	UPROPERTY()
	UscoreHud* Hud;

	AFlipperActor* leftFlipper;
	AFlipperActor* rightFlipper;
	UPROPERTY(EditAnywhere)
	Aplunger* plunger;
	UPROPERTY(EditAnywhere)
	float ballStartSpeed;
	int score;
	int ballsLeft;
	int multiBallAmount;
	UPROPERTY(EditAnywhere)
	bool bGameOver;
	bool bIsNudged;
	float nudgeTimer;
	FRotator defaultCamRot;
	int tickTime;
	float lightIdleTimer;
	int lightnum;
	bool bLightIdleStarted;
	bool bIsFirePressed;
	float fireTimer;
	TArray<ABall*> ballsOnScreen;
	//accessability properties
	UPROPERTY()
	EAssistLevel assistLevel = EAssistLevel::OFF;
	float interval;
	float assistTimer;
	bool bAutoFlip;
	void SetAssist1();
	void SetAssist2();
	void SetAssist3();
	void SetAssistOff();
	void changeAssist(EAssistLevel level);
};

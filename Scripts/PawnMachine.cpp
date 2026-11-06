// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnMachine.h"

#include "scoreHud.h"
#include "lightbulb.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APawnMachine::APawnMachine()
{
	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//input setup
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	//root component setup
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	//camera setup
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(-780.0f, 0.0f, 1200.0f));
	camera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//visual mesh setup
	visualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	visualMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tableModelVisualAsset(TEXT("/Script/Engine.StaticMesh'/Game/Models/TableModel.TableModel'"));
	if (tableModelVisualAsset.Succeeded())
	{
		visualMesh->SetStaticMesh(tableModelVisualAsset.Object);  
		visualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		visualMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
		visualMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	// material setup
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> tableFloorMat(TEXT("/Script/Engine.Material'/Game/Materials/tableFloor.tableFloor'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> tableWallsMat(TEXT("/Script/Engine.Material'/Game/Materials/tableWalls.tableWalls'"));
	if (tableFloorMat.Succeeded())
	{
		visualMesh->SetMaterial(0, tableFloorMat.Object);
	}
	if (tableWallsMat.Succeeded())
	{
		visualMesh->SetMaterial(1, tableWallsMat.Object);
	}
	//Pinball Spawn Location setup
	pinballSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Pinball Spawn Point"));
	pinballSpawnLocation->SetupAttachment(RootComponent);
	pinballSpawnLocation->SetRelativeLocation(FVector(-130.0f, 130.0f, 53.0f));
	ballStartSpeed = 160.0f;

	//Balls Left Setup
	ballsLeftSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Balls Left Spawn Point"));
	ballsLeftSpawnPoint->SetupAttachment(RootComponent);
	ballsLeftSpawnPoint->SetRelativeLocation(FVector::Zero());

	//multiball spawn point setup
	multiBallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Multiball Spawn Point"));
	multiBallSpawnPoint->SetupAttachment(RootComponent);
	multiBallSpawnPoint->SetRelativeLocation(FVector::Zero());
}

// Called when the game starts or when spawned
void APawnMachine::BeginPlay()
{
	Super::BeginPlay();
	bIsFirePressed = false;
	fireTimer = 0.0f;
	GetLights();
	bLightIdleStarted = false;
	ballsLeft = 3;
	multiBallAmount = 5;
	bGameOver = false;
	// spawns balls left on the machine
	for (int i = 0; i < ballsLeft; i++)
	{
		ABall* newBall = (ABall*)GWorld->SpawnActor(ABall::StaticClass());
		newBall->SetActorLocation(ballsLeftSpawnPoint->GetComponentLocation() - FVector(30 * i,0,0));
		ballsLeftArray.Add(newBall);
	}
	// find each flipper
	for (AFlipperActor* flipper : flippers)
	{
		if (flipper->flipperSide == EFlipperSide::LEFT)
		{
			leftFlipper = flipper;
		}
		if (flipper->flipperSide == EFlipperSide::RIGHT)
		{
			rightFlipper = flipper;
		}
	}
	// put score HUD on screen
	if (scoreHud)
	{
		Hud = CreateWidget<UscoreHud>(GetWorld(), scoreHud);
		if (Hud)
		{
			Hud->AddToViewport();
		}
	}
	
	defaultCamRot = camera->GetRelativeRotation();

	
}

// Called every frame
void APawnMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	tickTime = DeltaTime;
	// end the game when there are no balls left
	if (ballsOnScreen.Num() <= 0 && ballsLeft == 0)
	{
		EndGame();
	}
	// counts how long space is held for
	if (bIsFirePressed)
	{
		fireTimer += DeltaTime;
	}

	// Starts idle lights animation
	if (bGameOver)
	{
		if (!bLightIdleStarted)
		{
			for (Alightbulb* light : storedLights)
			{
				light->TurnOn();
			}
			bLightIdleStarted = true;
		}
		
		lightIdleTimer -= DeltaTime;
		if (lightIdleTimer <= 0)
		{
			storedLights[lightnum]->TurnOff();
			lightnum++;
			lightIdleTimer = 0.5f;
		}
		if (lightnum >= storedLights.Num())
		{
			lightnum = 0;

			bLightIdleStarted = false;
		}
		
	}

	// rotate camera on nudge, targetRotation set on nudge button press
	if(bIsNudged)
	{
		FRotator NewRotation = FMath::RInterpTo(camera->GetRelativeRotation(), targetRotation, DeltaTime, 2.0f);
		camera->SetRelativeRotation(NewRotation);
		nudgeTimer -= DeltaTime;
		if (nudgeTimer <= 0)
		{
			targetRotation = defaultCamRot;
		}
				
		FRotator newRotation = FMath::RInterpTo(camera->GetRelativeRotation(), targetRotation, DeltaTime, 2.0f);
		camera->SetRelativeRotation(newRotation);
		if (camera->GetRelativeRotation().Equals(defaultCamRot, 0.5))
		{
			camera->SetRelativeRotation(defaultCamRot);
			bIsNudged = false;
		}
			
	}
	
	// autoflips both flippers, for accessability requirement
	assistTimer -= DeltaTime;
	if (assistTimer <= 0 && assistLevel != EAssistLevel::OFF)
	{
		bAutoFlip = !bAutoFlip;
		assistTimer = interval;
			if (bAutoFlip)
			{
				
				LeftFlipperPress();
				RightFlipperPress();
				
				
			}
			else 
			{
				
				LeftFlipperRelease();
				RightFlipperRelease();
				
				
			}
		}
	}
	
// Destroy the ball when it falls off the table
void APawnMachine::KillBall(ABall* ball)
{
	
	ball->Destroy();
	ballsOnScreen.Remove(ball);
}
// Called when final ball falls off the table, restarts game
void APawnMachine::EndGame()
{
	if (Hud)
	{
		Hud->ShowRestartMessage();
	}
	bGameOver = true;
}

// Adds to the score, called when a bumper is hit by a ball
void APawnMachine::UpdateScore(int points)
{
	score += points;
	if (Hud)
	{
		Hud->SetScore(score);
	}
	if (score % 5 == 0 && score % 10 != 0)
	{
		for (Alightbulb* light : storedLights)
		{
			if (light && !bGameOver) {
				light->TurnOff();
				
			}
		}
	}
	else if (score % 10 == 0)
	{
		for (Alightbulb* light : storedLights)
		{
			if (light && !bGameOver)
			{
				light->TurnOn();
			}
		}
	}
}

// Called to bind functionality to input
void APawnMachine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("Fire",IE_Pressed, this, &APawnMachine::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &APawnMachine::FireReleased);
	InputComponent->BindAction("LeftFlipper", IE_Pressed, this, &APawnMachine::LeftFlipperPress);
	InputComponent->BindAction("LeftFlipper", IE_Released, this, &APawnMachine::LeftFlipperRelease);
	InputComponent->BindAction("RightFlipper", IE_Pressed, this, &APawnMachine::RightFlipperPress);
	InputComponent->BindAction("RightFlipper", IE_Released, this, &APawnMachine::RightFlipperRelease);
	InputComponent->BindAction("Restart", IE_Pressed, this, &APawnMachine::Restart);
	InputComponent->BindAction("NudgeLeft", IE_Pressed, this, &APawnMachine::NudgeLeft);
	InputComponent->BindAction("NudgeRight", IE_Pressed, this, &APawnMachine::NudgeRight);
	PlayerInputComponent->BindAction("AssistOne", IE_Pressed, this, &APawnMachine::SetAssist1);
	PlayerInputComponent->BindAction("AssistTwo", IE_Pressed, this, &APawnMachine::SetAssist2);
	PlayerInputComponent->BindAction("AssistThree", IE_Pressed, this, &APawnMachine::SetAssist3);
	PlayerInputComponent->BindAction("AssistOff", IE_Pressed, this, &APawnMachine::SetAssistOff);
}
// Checks if there are any balls left or in play, restarts timer while held
void APawnMachine::FirePressed()
{
	if (ballsLeft > 0 && ballsOnScreen.Num() == 0)
	{
		fireTimer = 0.0f;
		plunger->bPullBackPlunger = true;
		bIsFirePressed = true;
	}
	
	
}

//Called when space is released, Fires the ball at velocity based on how long space was held
void APawnMachine::FireReleased()
{
	
	if (ballsLeft > 0 && ballsOnScreen.Num() == 0) {
		ballsLeft -= 1;
		bIsFirePressed = false;
		plunger->bPullBackPlunger = false;
		plunger->bFirePlunger = true;
		ballStartSpeed = 550 * fireTimer;
		ballStartSpeed = FMath::Clamp(ballStartSpeed, 500, 1100);
		ABall* newBall = (ABall*)GWorld->SpawnActor(ABall::StaticClass());
		newBall->SetActorLocation(pinballSpawnLocation->GetComponentLocation());
		newBall->SetMachinePawnMiddle(this);
		newBall->velocity.X += ballStartSpeed;
		ballsOnScreen.Add(newBall);
		// removes ball from ammunition
		ABall* ballToDestroy = ballsLeftArray[0];
		ballsLeftArray.Remove(ballToDestroy);
		ballToDestroy->Destroy();
	}
}
//rotates camera and adds a force to the right when D key is pressed
void APawnMachine::NudgeRight()
{
	bIsNudged = true;
	nudgeTimer = 1.0f;
	targetRotation = defaultCamRot + FRotator(0.0f,0.0f,-15.0f);
	
	for (TObjectIterator<ABall> ball; ball; ++ball)
	{
		ABall* currentBall = *ball;
		if (currentBall->GetWorld() != GetWorld())
		{
			continue;
		}
		if (currentBall)
		{
			currentBall->velocity.Y += 75.0f;
		}
	}
}
//Rotates camera and adds a force to the left when A key is pressed
void APawnMachine::NudgeLeft()
{
	bIsNudged = true;
	nudgeTimer = 1.0f;
	 targetRotation = defaultCamRot + FRotator(0.0f, 0.0f, 15.0f);
	
	for (TObjectIterator<ABall> ball; ball; ++ball)
	{
		ABall* currentBall = *ball;
		if (currentBall->GetWorld() != GetWorld())
		{
			continue;
		}
		if (currentBall)
		{
			currentBall->velocity.Y -= 75.0f;
		}
	}
}
//flip flipper when left arrow is pressed
void APawnMachine::LeftFlipperPress()
{
	if (bGameOver)
	{
		return;
	}
	leftFlipper->OnFlipperPress();
}
// release flipper when left arrow is released
void APawnMachine::LeftFlipperRelease()
{
	if (bGameOver)
	{
		return;
	}
	leftFlipper->OnFlipperRelease();
}
// flip flipper when right arrow is pressed
void APawnMachine::RightFlipperPress()
{
	if (bGameOver)
	{
		return;
	}
	rightFlipper->OnFlipperPress();
}
// release flipper, called when right arrow is released
void APawnMachine::RightFlipperRelease()
{
	if (bGameOver)
	{
		return;
	}
	rightFlipper->OnFlipperRelease();
}
// Resets Score and ball amount
void APawnMachine::Restart()
{
	if (!bGameOver)
	{
		return;
	}
	else 
	{
		if (Hud)
		{
			for (TObjectIterator<ABall> ball; ball; ++ball)
			{
				ABall* currentBall = *ball;
				if (currentBall->GetWorld() != GetWorld())
				{
					continue;
				}
				if (currentBall)
				{
					currentBall->Destroy();
				}
			}
			Hud->HideRestartMessage();
			ballsLeft = 3;
			score = 0;
			Hud->SetScore(score);
			bGameOver = false;
			for (int i = 0; i < ballsLeft; i++)
			{
				ABall* newBall = (ABall*)GWorld->SpawnActor(ABall::StaticClass());
				newBall->SetActorLocation(ballsLeftSpawnPoint->GetComponentLocation() - FVector(30 * i, 0, 0));
				ballsLeftArray.Add(newBall);
			}
		}
	}
}
//set assist level 1
void APawnMachine::SetAssist1()
{
	changeAssist(EAssistLevel::ONE);
}
// set assist level 2
void APawnMachine::SetAssist2()
{
	changeAssist(EAssistLevel::TWO);
}
//set assist level 3
void APawnMachine::SetAssist3()
{
	changeAssist(EAssistLevel::THREE);
}

// Turn off assist
void APawnMachine::SetAssistOff()
{
	changeAssist(EAssistLevel::OFF);
}
// For Accessability, so players dont have to mash buttons to play, each level decreases interval bewteen flips, use buttons 1-4
void APawnMachine::changeAssist(EAssistLevel level)
{
	assistLevel = level;
	bAutoFlip = false;
	switch (level)
	{
	case EAssistLevel::OFF:
		interval = 0.0f;
		assistTimer = interval;

		break;
	case EAssistLevel::ONE:
		interval = 1.0f; 
		assistTimer = interval;

		break;
	case EAssistLevel::TWO:
		interval = 0.8f;
		assistTimer = interval;
	
		break;
	case EAssistLevel::THREE:
		interval = 0.5f; 
		assistTimer = interval; 
		
		break;
	}

	
}
// Gets the lights around the machine, each are numbered in the editor and sorted in this function
void APawnMachine::GetLights()
{
	
	for (TObjectIterator<Alightbulb> lightbulb; lightbulb; ++lightbulb)
	{
		Alightbulb* currentLight = *lightbulb;
		if (currentLight->GetWorld() != GetWorld())
		{
			continue;
		}
		else 
		{
			storedLights.Add(currentLight);
		}

	}
	storedLights.Sort([](const Alightbulb& A, const Alightbulb& B)
		{
			return A.lightNumber < B.lightNumber;
		});
}

//spawns balls from the top of the machine, randomising the Y velocity
void APawnMachine::MultiBall()
{
	for (int i = 0; i < multiBallAmount; i++)
	{
		ABall* newBall = (ABall*)GWorld->SpawnActor(ABall::StaticClass());
		newBall->SetMachinePawnMiddle(this);
		newBall->SetActorLocation(multiBallSpawnPoint->GetComponentLocation() - FVector(30 * i, 0, 0));
		ballsOnScreen.Add(newBall);
		newBall->velocity.Y += FMath::FRandRange(-150.0,150.0);
	}
}
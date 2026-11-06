// Fill out your copyright notice in the Description page of Project Settings.


#include "plunger.h"

// Sets default values
Aplunger::Aplunger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	plungerLength = 80;
	// setup root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	plungerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	plungerMesh->SetupAttachment(RootComponent);
	//setup plunger mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> plungerAsset(TEXT("/Script/Engine.StaticMesh'/Game/Models/PlungerModel.PlungerModel'"));
	if (plungerAsset.Succeeded())
	{
		plungerMesh->SetStaticMesh(plungerAsset.Object);
		plungerMesh->SetRelativeLocation(FVector(0, 0, 0));
		plungerMesh->SetRelativeRotation(FRotator(90, 0, 0));
		plungerMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	}
	//setup plunger head and body materials
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> plungerHeadMaterial(TEXT("/Script/Engine.Material'/Game/Materials/PlungerHead.PlungerHead'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> plungerBodyMaterial(TEXT("/Script/Engine.Material'/Game/Materials/PlungerBody.PlungerBody'"));
	if (plungerHeadMaterial.Succeeded())
	{
		plungerMesh->SetMaterial(0, plungerHeadMaterial.Object);
	}
	if (plungerBodyMaterial.Succeeded())
	{
		plungerMesh->SetMaterial(1, plungerBodyMaterial.Object);
	}

}

// Called when the game starts or when spawned
void Aplunger::BeginPlay()
{
	// find max pullback length
	Super::BeginPlay();
	startLocation = this->GetActorLocation();
	newLocation = startLocation;
	endLocation = startLocation;
	endLocation.X -= plungerLength;
}

// Called every frame
void Aplunger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bPullBackPlunger)
	{
		PullBack(DeltaTime);
	}
	if (bFirePlunger)
	{
		Fired(DeltaTime);
	}
	
		SetActorLocation(newLocation);
	
}

// pull back to max length, while space is held
void Aplunger::PullBack(float DeltaTime)
{
	
		newLocation = FMath::VInterpConstantTo(newLocation, endLocation, DeltaTime, pullBackSpeed);
		if (newLocation.Equals(endLocation, 0.1f))
		{
			bPullBackPlunger = false;
		}
	

}
// return plunger to start location when space is released
void Aplunger::Fired(float DeltaTime)
{
	
		newLocation = FMath::VInterpConstantTo(newLocation, startLocation, DeltaTime, firedSpeed);
		if (newLocation.Equals(startLocation, 0.1f))
		{
			bFirePlunger = false;
		}
	

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "wall.h"

// Sets default values
Awall::Awall()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	// set up component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT( "RootComponent"));
	wallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	wallMesh->SetupAttachment(RootComponent);
	
		static ConstructorHelpers::FObjectFinder<UStaticMesh> wallVisualMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (wallVisualMesh.Succeeded())
	{
		wallMesh->SetStaticMesh(wallVisualMesh.Object);
		wallMesh->SetWorldScale3D(FVector(0.2f, 1.0f, 0.2f));
		
	}
	
	
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetupAttachment(wallMesh);
	
	
	
	
}

// Called when the game starts or when spawned
void Awall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void Awall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



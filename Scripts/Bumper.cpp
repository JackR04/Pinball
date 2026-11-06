// Fill out your copyright notice in the Description page of Project Settings.


#include "Bumper.h"
#include "Ball.h"
#include "PawnMachine.h"
// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	bumperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	bumperMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bumperVisualMesh(TEXT("/Script/Engine.StaticMesh'/Game/Models/BumperModel.BumperModel'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> bumperMaterial(TEXT("/Script/Engine.Material'/Game/Materials/Bumper.Bumper'"));
	if (bumperVisualMesh.Succeeded())
	{
		bumperMesh->SetStaticMesh(bumperVisualMesh.Object);
		bumperMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		// set start size
		startSize = bumperMesh->GetRelativeScale3D();
		// set end size
		endSize = startSize * 1.5;
	}
	if (bumperMaterial.Succeeded())
	{
		bumperMesh->SetMaterial(0, bumperMaterial.Object);

	}
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	sphereCollider->SetupAttachment(RootComponent);
	sphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	sphereCollider->SetCollisionResponseToAllChannels(ECR_Block);
	

}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();
	targetSize = startSize;
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// grow from current size to target size
	FVector currentSize = bumperMesh->GetRelativeScale3D();
	FVector newSize = FMath::Lerp(currentSize, targetSize, 0.3f);
	bumperMesh->SetRelativeScale3D(newSize);

	// move back to start size
	if (newSize.Equals(endSize))
	{
		targetSize = startSize;
	}
	
}

// set target size to end size
void ABumper::Grow()
{
	if (machine) {
		targetSize = endSize;
		machine->UpdateScore(points);
	}
}



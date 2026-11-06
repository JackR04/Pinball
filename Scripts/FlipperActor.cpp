// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipperActor.h"
#include "Ball.h"

// Sets default values
AFlipperActor::AFlipperActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	flipperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	flipperMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> flipperVisualAsset(TEXT("/Script/Engine.StaticMesh'/Game/Models/FlipperModel.FlipperModel'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> flipperMaterial(TEXT("/Script/Engine.Material'/Game/Materials/Flipper.Flipper'"));
	if (flipperVisualAsset.Succeeded())
	{
		flipperMesh->SetStaticMesh(flipperVisualAsset.Object);
		flipperMesh->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
		flipperMesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.125f));
	}
	if (flipperMaterial.Succeeded())
	{
		flipperMesh->SetMaterial(0, flipperMaterial.Object);
	}

	overlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	overlapComponent->SetupAttachment(flipperMesh);
	

}

// Called when the game starts or when spawned
void AFlipperActor::BeginPlay()
{
	Super::BeginPlay()
	closedRotation = this->GetActorRotation();
	//sets open rotation based on side
	openRotation = closedRotation;
	openRotation.Yaw += (flipperSide == EFlipperSide::RIGHT) ? 60 : -60;
	targetRotation = closedRotation;
}

// Called every frame
void AFlipperActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator newRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, speed);
	SetActorRotation(newRotation);
	// flipper is no longer moving if close enough to target rotation
	if (GetActorRotation().Equals(openRotation, 0.05) || GetActorRotation().Equals(closedRotation, 0.05))
	{
		bIsMoving = false;
	}
	
}

// on left arrow press open flipper
void AFlipperActor::OnFlipperPress()
{
	bIsMoving = true;
	targetRotation = openRotation;
}
// oon left arrow release close flipper
void AFlipperActor::OnFlipperRelease()
{
	bIsMoving = false;
	targetRotation = closedRotation;
}



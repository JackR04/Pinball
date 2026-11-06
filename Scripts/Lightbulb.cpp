// Fill out your copyright notice in the Description page of Project Settings.


#include "lightbulb.h"
#include "PawnMachine.h"
// Sets default values
Alightbulb::Alightbulb()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	// setup root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	lightbulbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Mesh"));
	lightbulbMesh->SetupAttachment(RootComponent);
	// setup mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lightbulbAsset(TEXT("/Script/Engine.StaticMesh'/Game/Models/LightbulbModel.LightbulbModel'"));
	if (lightbulbAsset.Succeeded())
	{
		lightbulbMesh->SetStaticMesh(lightbulbAsset.Object);
		lightbulbMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		lightbulbMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
		lightbulbMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	// setup bulb and metal materials
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> lightbulbBulbMaterial(TEXT("/Script/Engine.Material'/Game/Materials/LightbulbBulb.LightbulbBulb'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> lightbulbMetalMaterial(TEXT("/Script/Engine.Material'/Game/Materials/Lightbulbmetal.Lightbulbmetal'"));
	if (lightbulbBulbMaterial.Succeeded())
	{
		lightbulbMesh->SetMaterial(0, lightbulbBulbMaterial.Object);
	}
	if (lightbulbMetalMaterial.Succeeded())
	{
		lightbulbMesh->SetMaterial(1, lightbulbMetalMaterial.Object);
	}
	
	bIsOff = false;
}

// Called when the game starts or when spawned
void Alightbulb::BeginPlay()
{
	// creates instance of bulb material to control emission
	bulbOffInstance = lightbulbMesh->CreateDynamicMaterialInstance(0);
	Super::BeginPlay();
	
}

// Called every frame
void Alightbulb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// increase emission
void Alightbulb::TurnOn()
{
	if (bulbOffInstance && bIsOff)
	{
		bulbOffInstance->SetScalarParameterValue(TEXT("EmissiveStrength"), 10.0f);
		bIsOff = false;
	}
}
//turn off emission
void Alightbulb::TurnOff()
{
	if (bulbOffInstance && !bIsOff)
	{
		bulbOffInstance->SetScalarParameterValue(TEXT("EmissiveStrength"), 0.0f);
		bIsOff = true;
	}
}

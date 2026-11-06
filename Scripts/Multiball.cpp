// Fill out your copyright notice in the Description page of Project Settings.


#include "multiBall.h"

// Sets default values
AmultiBall::AmultiBall()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	// setup hitbox for multiball
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxCollider->SetCollisionResponseToAllChannels(ECR_Block);

	boxCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AmultiBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AmultiBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


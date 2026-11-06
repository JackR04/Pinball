// Fill out your copyright notice in the Description page of Project Settings.


#include "killBox.h"


// Sets default values
AkillBox::AkillBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// setup kill box hit box
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxCollider->SetCollisionResponseToAllChannels(ECR_Block);

	boxCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AkillBox::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AkillBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Bumper.h"
#include "PawnMachine.h"
// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Sphere"));
	sphereCollider->InitSphereRadius(5.0f);
	sphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	sphereCollider->SetCollisionResponseToAllChannels(ECR_Block);
	sphereCollider->BodyInstance.SetUseCCD(true);
	RootComponent = sphereCollider;


    ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	ballMesh->SetupAttachment(RootComponent);
	ballMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ballVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (ballVisualAsset.Succeeded())
	{
		ballMesh->SetStaticMesh(ballVisualAsset.Object);
		ballMesh->SetRelativeLocation(FVector(0.0f));
		ballMesh->SetWorldScale3D(FVector(0.27f));
	}

	

	gravity = 50.0f;
	velocity = (FVector)(0.0f,0.0f,0.0f);
	 
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// go towards bottom of the machine
	velocity.X -= gravity * DeltaTime;
	// dont move up or down
	velocity.Z = 0;
	// persuade ball toward the middle
	velocity.Y += (this->GetActorLocation().Y < machineMiddle.Y) ? 2.0 : -2.0;
	//If stuck do a little jump up the machine
	if (velocity.IsNearlyZero())
	{
		
		velocity = FVector::ZeroVector;
		velocity.X += 500;
		return;
	}
	// calculate next location
	FVector newLocation = velocity * DeltaTime;
	FHitResult hit;
	//move to new location check if hit anything
	AddActorWorldOffset(newLocation, true, &hit);
	if (hit.bBlockingHit)
	{
		FVector normal = hit.ImpactNormal.GetSafeNormal();
		//ball kept getting stuck in bumpers and walls, move it away so it doesnt get stuck
		AddActorWorldOffset(FVector(normal.X, normal.Y, 0) * 2, false);
		AActor* hitActor = hit.GetActor();
		
		if (hitActor && hitActor->IsA(ABumper::StaticClass()))
		{
			//if hit a bumper grow bumper and rebound faster
			velocity = velocity.MirrorByVector(normal) * 1.1;
			
			if (ABumper* bumper = Cast<ABumper>(hitActor))
			{
				bumper->Grow();
			}
		}
		// if hit kill box destroy ball
		else if (hitActor && hitActor->IsA(AkillBox::StaticClass())) 
		{
			if (pawnMachine)
			{
				pawnMachine->KillBall(this);
			}
		}
		else if (hitActor && hitActor->IsA(AFlipperActor::StaticClass()))
		{
			
			if (AFlipperActor* flipper = Cast<AFlipperActor>(hitActor))
			{
				//if hit a flipper while moving rebound faster
				if (flipper->bIsMoving)
				{
					velocity = velocity.MirrorByVector(normal) * 1.4;
					return;
				}
				//if not moving rebound slower
				else { velocity = velocity.MirrorByVector(normal) * 0.5; return; }
			}
			
		}
		// hit multiball hitbox
		else if (hitActor && hitActor->IsA(AmultiBall::StaticClass()))
		{
			if (pawnMachine)
			{
				pawnMachine->KillBall(this);
				pawnMachine->MultiBall();
				
			}
		}
		// if hit a wall 
		else
		{
			velocity = velocity.MirrorByVector(normal) * 0.8;
		}
		
		
		
	}
	
}
// set by machine when new ball is spawned
void ABall::SetMachinePawnMiddle(AActor* machine)
{
	if (APawnMachine* tempMachine = Cast<APawnMachine>(machine))
	{
		machineMiddle = machine->GetActorLocation();
		pawnMachine = tempMachine;
	}

}





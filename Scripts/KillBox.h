// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "killBox.generated.h"

UCLASS()
class PINBALL_API AkillBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AkillBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UBoxComponent* boxCollider;

};

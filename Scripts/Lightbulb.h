// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "lightbulb.generated.h"

UCLASS()
class PINBALL_API Alightbulb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Alightbulb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* lightbulbMesh;
	UMaterialInstanceDynamic* bulbOffInstance;
	UPROPERTY(EditAnywhere)
	int lightNumber;
	bool bIsOff;
	void TurnOff();
	void TurnOn();
};

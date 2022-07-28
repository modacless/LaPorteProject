// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConnectedWire.h"
#include "GameFramework/Actor.h"
#include "ConnectedActor.generated.h"

UCLASS()
class LAPORTEPROJECT_API AConnectedActor : public AActor, public IConnectedWire
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConnectedActor();

	//Mesh component

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

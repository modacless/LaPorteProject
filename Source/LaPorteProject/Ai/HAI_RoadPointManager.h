// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAI_RoadPoint.h"
#include "GameFramework/Actor.h"
#include "HAI_RoadPointManager.generated.h"

UCLASS()
class LAPORTEPROJECT_API AHAI_RoadPointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHAI_RoadPointManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//All point of the road
	UPROPERTY(EditAnywhere, Category= Path)
	TArray<AHAI_RoadPoint*> RoadPoints;
	
	//Next Road
	UPROPERTY()
	AHAI_RoadPointManager* NextRoad;

};

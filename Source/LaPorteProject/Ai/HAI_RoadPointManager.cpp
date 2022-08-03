// Fill out your copyright notice in the Description page of Project Settings.


#include "HAI_RoadPointManager.h"

// Sets default values
AHAI_RoadPointManager::AHAI_RoadPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHAI_RoadPointManager::BeginPlay()
{
	Super::BeginPlay();

	for(USHORT i = 0; i< RoadPoints.Num(); i++)
	{
		RoadPoints[i]->PointManager = this;
	}
	
	if(ActualRoadPoint >= 0 && ActualRoadPoint < RoadPoints.Num())
	{
		ActualPoint = RoadPoints[ActualRoadPoint];
	}
}

// Called every frame
void AHAI_RoadPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHAI_RoadPointManager::NextPoint()
{
	ActualRoadPoint++;
	if(ActualRoadPoint < 0)
	{
		ActualRoadPoint = RoadPoints.Num()-1;
	}

	if(ActualRoadPoint > RoadPoints.Num()-1)
	{
		ActualRoadPoint = 0;
	}
	ActualPoint = RoadPoints[ActualRoadPoint];
}


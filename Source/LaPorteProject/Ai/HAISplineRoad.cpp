// Fill out your copyright notice in the Description page of Project Settings.


#include "HAISplineRoad.h"

// Sets default values
AHAISplineRoad::AHAISplineRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline road"));

	if(SplineComponent)
	{
		SetRootComponent(SplineComponent);  
	}  
}

// Called when the game starts or when spawned
void AHAISplineRoad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHAISplineRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


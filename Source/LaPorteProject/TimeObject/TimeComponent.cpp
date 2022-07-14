// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeComponent.h"


#pragma region UE4_Base

// Sets default values for this component's properties
UTimeComponent::UTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#pragma endregion UE4_Base

void UTimeComponent::InitTimeComponent(FTransform DayPosition, FTransform NightPosition)
{
	//PosAccordingTime.Add()
}


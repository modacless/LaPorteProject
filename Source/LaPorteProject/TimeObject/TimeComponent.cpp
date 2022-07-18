// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeComponent.h"
#pragma region UE4_Base

// Sets default values for this component's properties
UTimeComponent::UTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	if(PosAccordingTime.Num() == 0)
	{
		PosAccordingTime.Add(ETimeInDay::Day,FTransform());
		PosAccordingTime.Add(ETimeInDay::Night,FTransform());
	}

	if(PosAccordingTime.Num() != 2)
	{
		PosAccordingTime.Reset();
		PosAccordingTime.Add(ETimeInDay::Day,FTransform());
		PosAccordingTime.Add(ETimeInDay::Night,FTransform());
	}


}


// Called when the game starts
void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	UHGameInstance *GameInstance = Cast<UHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->ObjectsAccordingTime.Add(this);
	
}


// Called every frame
void UTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


#pragma endregion UE4_Base

void UTimeComponent::InitTimeComponent()
{

}


void UTimeComponent::ChangeTransformAccordingTime_Implementation(const ETimeInDay TimeToChange)
{
	if(TimeToChange == ETimeInDay::Day)
	{
		TimeDay.Broadcast();
	}
	if(TimeToChange == ETimeInDay::Night)
	{
		TimeNight.Broadcast();
	}
}

void UTimeComponent::ChangeToDay(const FTransform Transform)
{
	PosAccordingTime[ETimeInDay::Day] = Transform;
	TimeNight.Broadcast();
}

void UTimeComponent::ChangeToNight(const FTransform Transform)
{
	PosAccordingTime[ETimeInDay::Night] = Transform;
	TimeNight.Broadcast();
}

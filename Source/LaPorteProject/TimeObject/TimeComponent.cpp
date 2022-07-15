// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeComponent.h"

#pragma region UE4_Base

// Sets default values for this component's properties
UTimeComponent::UTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	UHGameInstance *GameInstance = Cast<UHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->ObjectsAccordingTime.Add(this);
	//OwnerTransform = GetOwner()->GetTransform();
	
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

void UTimeComponent::ChangeTransformAccordingTime(const ETimeInDay TimeToChange)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("BOUGE Object")));	
	GetOwner()->SetActorLocation(PosAccordingTime[TimeToChange].GetLocation()) ;
}


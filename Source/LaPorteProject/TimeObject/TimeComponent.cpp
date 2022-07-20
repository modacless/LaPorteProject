// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeComponent.h"

#include "AbstractTimeObject.h"
#include "Algo/Transform.h"
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
	if(GetOwner()->GetClass()->IsChildOf(AAbstractTimeObject::StaticClass()))
	{
		if(TimeToChange == ETimeInDay::Day)
		{
			TimeDay.Broadcast();
		}
		if(TimeToChange == ETimeInDay::Night)
		{
			TimeNight.Broadcast();
		}
	}else
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
	
}

void UTimeComponent::ChangeToDay(FTransform Transform)
{
	if(GetOwner()->GetClass()->IsChildOf(AAbstractTimeObject::StaticClass()))
	{
		PosAccordingTime[ETimeInDay::Day] = Transform;
	
		if(DayGhostActor == nullptr)
		{
			FActorSpawnParameters SpawnInfo;
			DayGhostActor = GetWorld()->SpawnActor<AGhostActor>(AGhostActor::StaticClass(),PosAccordingTime[ETimeInDay::Day].GetLocation(),PosAccordingTime[ETimeInDay::Day].GetRotation().Rotator(),SpawnInfo);
			DayGhostActor->GhostMesh->SetStaticMesh(Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))->GetStaticMesh());
			DayGhostActor->ChangeMaterial(ETimeInDay::Day);
		}
		
		TimeNight.Broadcast();
		DayGhostActor->RealObject = Cast<AAbstractTimeObject>(GetOwner());
		DayGhostActor->SetActorLocation(PosAccordingTime[ETimeInDay::Day].GetLocation());
		DayGhostActor->SetActorRotation(PosAccordingTime[ETimeInDay::Day].GetRotation().Rotator());
	}else
	{
		PosAccordingTime[ETimeInDay::Day] = Transform;
	}
	
}

void UTimeComponent::ChangeToNight(FTransform Transform)
{
	if(GetOwner()->GetClass()->IsChildOf(AAbstractTimeObject::StaticClass()))
	{
		PosAccordingTime[ETimeInDay::Night] = Transform;
	
		if(NightGhostActor == nullptr)
		{
			FActorSpawnParameters SpawnInfo;
			NightGhostActor = GetWorld()->SpawnActor<AGhostActor>(AGhostActor::StaticClass(),PosAccordingTime[ETimeInDay::Night].GetLocation(),PosAccordingTime[ETimeInDay::Night].GetRotation().Rotator(),SpawnInfo);
			NightGhostActor->GhostMesh->SetStaticMesh(Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))->GetStaticMesh());
			NightGhostActor->ChangeMaterial(ETimeInDay::Night);
		}
	
	
		TimeNight.Broadcast();
		NightGhostActor->RealObject = Cast<AAbstractTimeObject>(GetOwner());
		NightGhostActor->SetActorLocation(PosAccordingTime[ETimeInDay::Night].GetLocation());
		NightGhostActor->SetActorRotation(PosAccordingTime[ETimeInDay::Night].GetRotation().Rotator());
	}else
	{
		PosAccordingTime[ETimeInDay::Night] = Transform;
	}
}


void UTimeComponent::DestroyDay()
{
	if(DayGhostActor != nullptr)
	{
		PosAccordingTime[ETimeInDay::Day] = FTransform::Identity;
		GetWorld()->DestroyActor(DayGhostActor);
		DayGhostActor = nullptr;
	}

}

void UTimeComponent::DestroyNight()
{
	if(NightGhostActor != nullptr)
	{
		PosAccordingTime[ETimeInDay::Night] = FTransform::Identity;
		GetWorld()->DestroyActor(NightGhostActor);
		NightGhostActor = nullptr;
	}

}



// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractTimeObject.h"

// Sets default values
AAbstractTimeObject::AAbstractTimeObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OwnActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Mesh"));
	TimeComponent = CreateDefaultSubobject<UTimeComponent>(TEXT("TimeComponent"));
	
	DayMeshLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DayMesh Location"));
	NightMeshLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NightMesh Location"));

	TimeComponent->TimeDay.Clear();
	TimeComponent->TimeNight.Clear();
	
	TimeComponent->TimeDay.AddDynamic(this,&AAbstractTimeObject::ChangeDay);
	TimeComponent->TimeNight.AddDynamic(this,&AAbstractTimeObject::ChangeNight);
}

// Called when the game starts or when spawned
void AAbstractTimeObject::BeginPlay()
{
	Super::BeginPlay();

	/*TimeComponent->TimeDay.Clear();
	TimeComponent->TimeNight.Clear();
	
	TimeComponent->TimeDay.AddDynamic(this,&AAbstractTimeObject::ChangeDay);
	TimeComponent->TimeNight.AddDynamic(this,&AAbstractTimeObject::ChangeNight);*/

}

// Called every frame
void AAbstractTimeObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbstractTimeObject::ChangeNight_Implementation()
{
	this->SetActorLocation(TimeComponent->PosAccordingTime[ETimeInDay::Night].GetLocation()) ;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Night %s"),*TimeComponent->PosAccordingTime[ETimeInDay::Night].GetLocation().ToString()));
}

void AAbstractTimeObject::ChangeDay_Implementation()
{
	this->SetActorLocation(TimeComponent->PosAccordingTime[ETimeInDay::Day].GetLocation());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Day %s"),*TimeComponent->PosAccordingTime[ETimeInDay::Day].GetLocation().ToString()));
}


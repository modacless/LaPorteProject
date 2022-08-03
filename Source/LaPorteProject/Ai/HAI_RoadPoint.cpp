// Fill out your copyright notice in the Description page of Project Settings.


#include "HAI_RoadPoint.h"

// Sets default values
AHAI_RoadPoint::AHAI_RoadPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHAI_RoadPoint::OnAiEnter);
}

// Called when the game starts or when spawned
void AHAI_RoadPoint::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AHAI_RoadPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHAI_RoadPoint::OnAiEnter_Implementation(UPrimitiveComponent* OverlapComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AHAi* HorrorBot = nullptr;
	if(OtherActor != nullptr)
	{
		HorrorBot = Cast<AHAi>(OtherActor);
		if(HorrorBot != nullptr)
		{
			HorrorBot->ActualRoad->NextPoint();
		}
	}
}


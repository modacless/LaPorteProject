// Fill out your copyright notice in the Description page of Project Settings.


#include "HAI_RoadPoint.h"

// Sets default values
AHAI_RoadPoint::AHAI_RoadPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHAI_RoadPoint::OnAiEnter);
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &AHAI_RoadPoint::OnAiExit);

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

void AHAI_RoadPoint::OnAiExit_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	AHAi* HorrorBot = nullptr;
	if(OtherActor != nullptr)
	{
		HorrorBot = Cast<AHAi>(OtherActor);
		if(HorrorBot != nullptr && HorrorBot->ActualRoad == PointManager && OtherActor == HorrorBot)
		{
			isEnter = false;
		}
	}
}

void AHAI_RoadPoint::OnAiEnter_Implementation(UPrimitiveComponent* OverlapComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AHAi* HorrorBot = nullptr;
	if(OtherActor != nullptr && !isEnter)
	{
		HorrorBot = Cast<AHAi>(OtherActor);
		if(HorrorBot != nullptr && HorrorBot->ActualRoad == PointManager && OtherActor == HorrorBot)
		{
			HorrorBot->ActualRoad->NextPoint();
			isEnter = true;
		}
	}
}


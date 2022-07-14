// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeComponent.generated.h"

UENUM()
enum class ETimeInDay : uint8
{
	Day = 0,
	Night = 1
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LAPORTEPROJECT_API UTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = MapsAndSets,EditAnywhere)
	TMap<ETimeInDay,FTransform> PosAccordingTime;
	
private:
	void InitTimeComponent(FTransform DayPosition, FTransform NightPosition);
};

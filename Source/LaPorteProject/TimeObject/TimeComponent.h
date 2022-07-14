// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../World/HGameInstance.h"
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

	FTransform OwnerTransform;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ChangeTransformAccordingTime(const ETimeInDay TimeToChange);
	
	UPROPERTY(Category = "Time",EditAnywhere)
	TMap<ETimeInDay,FTransform> PosAccordingTime;

	
	
private:
	void InitTimeComponent(FTransform DayPosition, FTransform NightPosition);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AGhostActor;

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Editor.h"
#include "../World/HGameInstance.h"
#include "LaPorteProject/Tools/GhostActor.h"
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

	UFUNCTION(BlueprintNativeEvent)
	void ChangeTransformAccordingTime(const ETimeInDay TimeToChange);
	
	UPROPERTY(Category = "Time",EditAnywhere, BlueprintReadWrite)
	TMap<ETimeInDay,FTransform> PosAccordingTime;

	//Change time position, and declare
	UFUNCTION(BlueprintCallable, Category = "Time")
	void ChangeToDay(FTransform Transform);

	UFUNCTION(BlueprintCallable, Category = "Time")
	void ChangeToNight(FTransform Transform);

	UFUNCTION(BlueprintCallable, Category = "Time")
	void DestroyDay();

	UFUNCTION(BlueprintCallable, Category = "Time")
	void DestroyNight();

	//Event and delegate
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimeDay);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimeNight);
	
	UPROPERTY(BlueprintReadWrite)
	FTimeDay TimeDay;

	UPROPERTY(BlueprintReadWrite)
	FTimeNight TimeNight;

	//Reference ghost object time and night
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGhostActor *DayGhostActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGhostActor *NightGhostActor;
	
	
	
private:
	void InitTimeComponent();
};


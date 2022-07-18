// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeComponent.h"
#include "AbstractTimeObject.generated.h"

UCLASS()
class LAPORTEPROJECT_API AAbstractTimeObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbstractTimeObject();

	UPROPERTY(EditAnywhere)
	UTimeComponent *TimeComponent;

	//Onw mesh reference
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent *OwnActorMesh;

	//Reference ghost object time and night
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *DayMeshLocation;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *NightMeshLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void ChangeDay();

	UFUNCTION(BlueprintNativeEvent)
	void ChangeNight();

};

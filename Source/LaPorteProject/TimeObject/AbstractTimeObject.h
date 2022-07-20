// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Tools/GhostActor.h"
#include "TimeComponent.h"
#include "AbstractTimeObject.generated.h"

class AGhostActor;
UCLASS()
class LAPORTEPROJECT_API AAbstractTimeObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbstractTimeObject();

	UPROPERTY(EditAnywhere)
	UTimeComponent *TimeComponent = nullptr;

	//Onw mesh reference
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent *OwnActorMesh = nullptr;

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

	UFUNCTION(BlueprintCallable)
	void InitGhostActorDay(AGhostActor *Day);

	UFUNCTION(BlueprintCallable)
	void InitGhostActorNight(AGhostActor *Night);

};

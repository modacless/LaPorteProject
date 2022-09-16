// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HAi.h"
#include "HAI_RoadPoint.generated.h"

UCLASS()
class LAPORTEPROJECT_API AHAI_RoadPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHAI_RoadPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	bool IsEnter;
	
	UPROPERTY()
	UBoxComponent* BoxTrigger;

	UPROPERTY()
	AHAI_RoadPoint* NextPoint;

	UPROPERTY()
	class AHAI_RoadPointManager* PointManager;

	//Function
	UFUNCTION(BlueprintNativeEvent)
	void OnAiEnter(UPrimitiveComponent* OverlapComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION(BlueprintNativeEvent)
	void OnAiExit(UPrimitiveComponent* OverlappedComp,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void AiEnterDelay(AActor* OtherActor);
};

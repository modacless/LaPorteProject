// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "HAI_Controller.h"
#include "HAI_RoadPointManager.h"
#include "../TimeObject/AInterractable.h"
#include "HAi.generated.h"

UCLASS()
class LAPORTEPROJECT_API AHAi : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHAi();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Function
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ChangeToNextRoad();

	UFUNCTION(BlueprintImplementableEvent)
	void RotateHead();

	//Propriety
	UPROPERTY(EditAnywhere,Category= AI)
    class UBehaviorTree *BehaviorTree;

	UPROPERTY(EditAnywhere, Category= AI)
	class AHAI_RoadPointManager* ActualRoad;

	UPROPERTY(EditAnywhere, Category= AI)
	TMap<FString,AHAI_RoadPointManager*> AllRoads;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollisionHide = nullptr;

	UFUNCTION()
	void BeginOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TArray<AAInterractable*> HidingPlaceToCheck;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "HAI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class LAPORTEPROJECT_API AHAI_Controller : public AAIController
{
	GENERATED_BODY()

public:

	AHAI_Controller();

	virtual void BeginPlay() override;
	
	//Fonction
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	//Perception
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightRadius = 500.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AILostSightRadius = 100.f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightFieldOfView = 120.f;

	//Component
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	UAISenseConfig_Sight* AiSenseConfig;
};

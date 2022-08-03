// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HAi.h"
#include "Perception/AIPerceptionComponent.h"
#include "LaPorteProject/Player/HPlayer.h"
#include "Perception/AISenseConfig_Sight.h"
#include "HAI_Controller.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Road       UMETA(DisplayName="Follow Road"),
	Detected        UMETA(DisplayName="Detect Player"),
	LookFor UMETA(DisplayName="Look for Player"),
};


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

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;
	
	//Fonction
	UFUNCTION()
	void OnPawnDetected(AActor* DetectedPawns,FAIStimulus Stimulus);

	UFUNCTION()
	void MoveToNextPoint();

	UFUNCTION()
	void MoveToPlayer();

	UFUNCTION()
	void ChangeRoad();
	

	//Perception
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightRadius = 500.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AILostSightRadius = 100.f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightFieldOfView = 120.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	bool IsPlayerDetected = false;

	//State of enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState EnemyState;

	//Component
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	UAISenseConfig_Sight* AiSenseConfig;

	UPROPERTY()
	class AHAi* PawnAi;

	UPROPERTY()
	AActor* APlayer;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HAi.h"
#include "Perception/AIPerceptionComponent.h"
#include "LaPorteProject/Player/HPlayer.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "HAI_Controller.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Road       UMETA(DisplayName="Follow Road"),
	Detected   UMETA(DisplayName="Detect Player"),
	LookFor	   UMETA(DisplayName="Look for Player"),
	HearSound  UMETA(DisplayName ="Hear sound"),
};


/**
 * 
 */
UCLASS()
class LAPORTEPROJECT_API AHAI_Controller : public AAIController
{
	GENERATED_BODY()

protected:

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

	UFUNCTION(BlueprintCallable)
	void ChangeRoad(FString RoadName);

	UFUNCTION()
	void MoveToSound();

	UFUNCTION()
	void TimerLookingFor(float LookforTime);

	UFUNCTION()
	void MoveToFind();

	//Propriety
	//Enemy Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float EnemyRunSpeed = 600.f;
	
	//Perception
	//Timer
	FTimerHandle TimerToLookingFor;
	FTimerDelegate DelegateToLookingFor;
	
	//Sight
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightRadius = 500.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AILostSightRadius = 100.f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightFieldOfView = 120.f;

	//Hearing
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AIHearingRadius = 3000.f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	bool IsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere)
	FVector SoundPosition;

	//State of enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere)
	float TimeInStateLookingFor = 10.f; // Time in second during ai searching player

	//Component
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	UAISenseConfig_Sight* AiSenseConfig;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	UAISenseConfig_Hearing* AiHearingConfig;

	UPROPERTY()
	class AHAi* PawnAi;

	UPROPERTY()
	AActor* APlayer;

	UPROPERTY()
	UCharacterMovementComponent *OwnUcharacterMovement;

};

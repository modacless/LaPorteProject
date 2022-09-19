// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HAi.h"
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
	CheckHide UMETA(DisplayName = "Check hiding place"),
	CheckAround UMETA(DisplayName = "Check around"),
	CheckInsideHidePlace UMETA(DisplayName = "Check inside hinding place"),
	KillPlayer UMETA(DisplayName = "KillPlayer")
};

UCLASS()
class LAPORTEPROJECT_API AHAI_Controller : public AAIController
{
	GENERATED_BODY()

protected:
	bool CanOpenDoor = true;

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
	void LookFor(); //Stop to see player find place to hide

	UFUNCTION()
	void CheckHide();

	UFUNCTION()
	void CheckAround(); // Check around enemy

	UFUNCTION()
	void CheckInsideHide(); //Check inside hideout

	UFUNCTION()
	void OpenDoor();
	
	//Timer
	UFUNCTION()
	void TimerLookingFor(float LookforTime);

	UFUNCTION()
	void TimerOpenDoor(float TimeToOpen);

	UFUNCTION()
	void TimerBackToRoad(float timeBeforeRoad);
	
	//Propriety
	//Enemy Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float EnemyRunSpeed = 450.f;
	
	//Perception
	//Timer
	FTimerHandle TimerToLookingFor;
	FTimerDelegate DelegateToLookingFor;

	FTimerHandle TimerToOpenDoor;
	FTimerDelegate DelegateToOpenDoor;
	
	FTimerHandle TimerToGoBackToRoad;
	FTimerDelegate DelegateToGoBackToRoad;
	
	//Sight
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= AI)
	float AISightRadius = 750.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= AI)
	float AILostSightRadius = 800.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= AI)
	float AISightFieldOfView = 120.f;
	
	UPROPERTY()
	AActor* ObjectToCheck;

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
	float TimeInStateLookingFor = 5.f; // Time in second during ai searching player
	UPROPERTY(EditAnywhere)
	float TimeBeforeGoingBackRoad = 10.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= AI)
	UAIPerceptionComponent* AiPerception;
	
	UPROPERTY()
	class AHAi* PawnAi;

	UPROPERTY()
	USkeletalMeshComponent* PawnMesh;

	UPROPERTY()
	AActor* APlayer;

	UPROPERTY(BlueprintReadWrite)
	UCharacterMovementComponent *OwnUcharacterMovement;
	
	
};


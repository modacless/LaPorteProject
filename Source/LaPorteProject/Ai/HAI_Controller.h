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

	UFUNCTION()
	void BeginOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapHidingPlace(UPrimitiveComponent* OverlappedComp,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Timer
	UFUNCTION()
	void TimerLookingFor(float LookforTime);

	UFUNCTION()
	void TimerOpenDoor(float TimeToOpen);
	
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
	
	//Sight
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightRadius = 550.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AILostSightRadius = 550.f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	float AISightFieldOfView = 120.f;

	UPROPERTY()
	TArray<AAInterractable*> HidingPlaceToCheck;
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

	//Component
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	UAISenseConfig_Sight* AiSenseConfig;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= AI)
	UAISenseConfig_Hearing* AiHearingConfig;

	UPROPERTY()
	class AHAi* PawnAi;

	UPROPERTY()
	USkeletalMeshComponent* PawnMesh;

	UPROPERTY()
	AActor* APlayer;

	UPROPERTY()
	UCharacterMovementComponent *OwnUcharacterMovement;
	
	
};


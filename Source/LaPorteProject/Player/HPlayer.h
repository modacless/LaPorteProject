// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../TimeObject/TimeComponent.h"
#include "../World/HGameInstance.h"
#include "HPlayer.generated.h"


UENUM()
enum class EPlayerMovement : uint8 
{
	Run = 1 UMETA(DisplayName = "Run"),
	Walk  = 0   UMETA(DisplayName = "Walk"),
};

UCLASS()
class LAPORTEPROJECT_API AHPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Switch player State
	void ChangeStateMovement(EPlayerMovement State);

	//Manage stamina of player
	UFUNCTION()
	void ManageStamina(float Delta);

	UFUNCTION()
	void CanRunNow();

	//References
	UPROPERTY()
	UCharacterMovementComponent *OwnUcharacterMovement;

	UPROPERTY(BlueprintReadOnly)
	UHGameInstance* HGameInstance; //Actual game Instance

	//Property
	bool CanRun;

	//Stamina
	float ActualStamina;
	FTimerHandle TimerStamina;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Called when Axis Forward is used
	UFUNCTION()
	void MoveForward(float Value);
	
	//Called when Axis Right is used
	UFUNCTION()
	void MoveRight(float Value);

	//Called when pressed Run Button
	UFUNCTION()
	void StartRun();
	
	//Called when pressed Run Button
	UFUNCTION()
	void StopRun();

	//Action
	void TimeChange(ETimeInDay TimeToChange) const;

	void TargetObject(float Range);

	//Will change
	void StartTime()
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("BOUGE Object")));
		if(HGameInstance->ActualTimeInGame == ETimeInDay::Day)
		{
			TimeChange(ETimeInDay::Night);
			HGameInstance->ActualTimeInGame = ETimeInDay::Night;
		}else
		{
			TimeChange(ETimeInDay::Day);
			HGameInstance->ActualTimeInGame = ETimeInDay::Day;
		}
		
	};

	//Property//

	//Movement
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool IsRunning;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Data)
	float SpeedWalk;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Data)
	float SpeedRun;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EPlayerMovement PlayerMovement;

	//Stamina
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Data)
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Data)
	float GainStaminaPerSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Data)
	float UseStaminaPerSeconds;

	//Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Data)
	float RangeInterraction;
	
};

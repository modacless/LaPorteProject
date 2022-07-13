// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	void ChangeState(EPlayerMovement State);

	//References
	UPROPERTY()
	UCharacterMovementComponent *OwnUcharacterMovement;

	//Property
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float ActualVelocity;

	float ActualStamina;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly,Category = Data)
	float MaxStamina;
	

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

	//Property//
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool IsRunning;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Data)
	float SpeedWalk;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Data)
	float SpeedRun;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EPlayerMovement PlayerMovement;
	
};

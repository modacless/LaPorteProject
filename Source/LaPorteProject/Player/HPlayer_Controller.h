// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HPlayer_Controller.generated.h"

/**
 * 
 */
UCLASS()
class LAPORTEPROJECT_API AHPlayer_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void LookWatchRotation(float LerpValue,FRotator StartRotation ,FRotator GoalRotation);
	
	
};

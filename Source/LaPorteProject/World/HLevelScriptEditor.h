// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LaPorteProject/Ai/HAI_Controller.h"
#include "HLevelScriptEditor.generated.h"

/**
 * 
 */
UCLASS()
class LAPORTEPROJECT_API AHLevelScriptEditor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	//Function
	UFUNCTION(BlueprintCallable)
	void AiChangeRoad(FString RoadName)
	{
		AiController->ChangeRoad(RoadName);
	}
	
	//Propriety
	UPROPERTY(EditAnywhere, Category= AI)
	AHAI_Controller * AiController;

	
	
};

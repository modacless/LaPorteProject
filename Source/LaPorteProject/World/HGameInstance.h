// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HGameInstance.generated.h"

/**
 * 
 */
class UTimeComponent;
enum class ETimeInDay : uint8;

UCLASS()
class LAPORTEPROJECT_API  UHGameInstance final : public UGameInstance 
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<UTimeComponent*> ObjectsAccordingTime;
	ETimeInDay ActualTimeInGame;
	
};

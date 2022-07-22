// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInterractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LAPORTEPROJECT_API IInterractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent,Category = "Interract")
	void Interract();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interract")
	void StopInterract();
};

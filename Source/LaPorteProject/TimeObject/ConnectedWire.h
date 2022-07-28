// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConnectedWire.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConnectedWire : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LAPORTEPROJECT_API IConnectedWire
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent,Category = "Connected")
	void ConnectedUse();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};

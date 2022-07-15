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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

protected:
	virtual void Interract() = 0;
public:
};

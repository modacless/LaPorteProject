// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInterractable.h"
#include "GameFramework/Actor.h"
#include "AInterractable.generated.h"

UCLASS()
class LAPORTEPROJECT_API AAInterractable : public AActor, public IInterractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAInterractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//void Interract_Implementation() override;

};

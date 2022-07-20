// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../TimeObject/TimeComponent.h"
#include "LaPorteProject/TimeObject/AbstractTimeObject.h"
#include "GhostActor.generated.h"

enum class ETimeInDay : uint8;
class AAbstractTimeObject;

UCLASS()
class LAPORTEPROJECT_API AGhostActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GhostMesh;

	void ChangeMaterial(ETimeInDay CorrespondingTime);

	UPROPERTY(EditAnywhere)
	AAbstractTimeObject* RealObject;

};

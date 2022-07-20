// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostActor.h"

#pragma region Load
template <typename ObjClass>
FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
{
	if(Path == NAME_None) return NULL;
	//~
	
	return Cast<ObjClass>(StaticLoadObject( ObjClass::StaticClass(), NULL,*Path.ToString()));
}

FORCEINLINE UMaterialInstance* LoadMatFromPath(const FName& Path)
{
	if(Path == NAME_None) return NULL;
	//~
	
	return LoadObjFromPath<UMaterialInstance>(Path);
}
#pragma endregion  Load

// Sets default values
AGhostActor::AGhostActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GhostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Mesh"));
	
}

// Called when the game starts or when spawned
void AGhostActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
}

// Called every frame
void AGhostActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGhostActor::ChangeMaterial(ETimeInDay CorrespondingTime)
{
	if(CorrespondingTime == ETimeInDay::Day)
	{
		GhostMesh->SetMaterial(0,LoadMatFromPath("MaterialInstanceConstant'/Game/BluePrints/Tools/Materials/M_Ghost_Day.M_Ghost_Day'"));
	}

	if(CorrespondingTime == ETimeInDay::Night)
	{
		GhostMesh->SetMaterial(0,LoadMatFromPath("MaterialInstanceConstant'/Game/BluePrints/Tools/Materials/M_Ghost_Night.M_Ghost_Night'"));
	}
}






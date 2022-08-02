// Fill out your copyright notice in the Description page of Project Settings.


#include "HAI_Controller.h"

AHAI_Controller::AHAI_Controller()
{
	AiSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sensor Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception component")));
	if(AiSenseConfig)
	{
		AILostSightRadius += AISightRadius;
		
		AiSenseConfig->SightRadius = AISightRadius;
		AiSenseConfig->LoseSightRadius = AILostSightRadius;
		AiSenseConfig->PeripheralVisionAngleDegrees = AISightFieldOfView;
		AiSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		AiSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AiSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;

		GetPerceptionComponent()->SetDominantSense(*AiSenseConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this,&AHAI_Controller::OnPawnDetected);
		GetPerceptionComponent()->ConfigureSense(*AiSenseConfig);
	}


}

void AHAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	

}

void AHAI_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
}


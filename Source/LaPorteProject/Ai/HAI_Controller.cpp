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
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AHAI_Controller::OnPawnDetected);
		//GetPerceptionComponent()->OnTargetPerceptionUpdated(this,)
		GetPerceptionComponent()->ConfigureSense(*AiSenseConfig);
	}
}

void AHAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	PawnAi = Cast<AHAi>(GetPawn());
	EnemyState = EEnemyState::Road;
	APlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
}

void AHAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (EnemyState)
	{
	case EEnemyState::Detected:
		MoveToPlayer();
		break;
	case EEnemyState::Road:
		MoveToNextPoint();
		break;
	case EEnemyState::LookFor:
		break;
	}
}

FRotator AHAI_Controller::GetControlRotation() const
{
	if(GetPawn() == nullptr)
	{
		return FRotator(0,0,0);
	}

	return FRotator(0.0f,GetPawn()->GetActorRotation().Yaw,0.0f);
}

void AHAI_Controller::OnPawnDetected(AActor* SensedActor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{

		const AHPlayer* Player = Cast<AHPlayer>(SensedActor);
		if(Player != nullptr)
		{
			EnemyState = EEnemyState::Detected;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Find!"));	
		}
	}else
	{
		EnemyState = EEnemyState::Road;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Road!"));
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Sensing Actor"));
	/*if(DetectedPawns.Num() > 0)
	{
		
		for(USHORT i = 0; i< DetectedPawns.Num(); i++)
		{
			const AHPlayer* Player = Cast<AHPlayer>(DetectedPawns[i]);
			if(Player != nullptr)
			{
				float Distance = GetPawn()->GetDistanceTo(Player);
				EnemyState = EEnemyState::Detected;
			}
			
		}
	}else
	{
		
		EnemyState = EEnemyState::Road;
	}
	*/
}

void AHAI_Controller::MoveToNextPoint()
{
	if(PawnAi->ActualRoad != nullptr && PawnAi->ActualRoad->ActualPoint != nullptr)
	{
		MoveToActor(PawnAi->ActualRoad->ActualPoint);
	}
}

void AHAI_Controller::MoveToPlayer()
{
	//GetPawn()->
	MoveToActor(APlayer);
}

void AHAI_Controller::ChangeRoad()
{
	
}




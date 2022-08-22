// Fill out your copyright notice in the Description page of Project Settings.


#include "HAI_Controller.h"

#include "Perception/AISenseConfig_Hearing.h"


AHAI_Controller::AHAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;
	AiSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sensor Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception component")));
	AiHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Component"));
	if(AiSenseConfig)
	{
		AILostSightRadius += AISightRadius;
		
		AiSenseConfig->SightRadius = AISightRadius;
		AiSenseConfig->LoseSightRadius = AILostSightRadius;
		AiSenseConfig->PeripheralVisionAngleDegrees = AISightFieldOfView;
		AiSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		AiSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AiSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
		//GetPerceptionComponent()->OnTargetPerceptionUpdated(this,)
		//
	}

	if(AiHearingConfig)
	{
		AiHearingConfig->HearingRange = AIHearingRadius;
		AiHearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		AiHearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AiHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		
	}

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AHAI_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*AiSenseConfig);
	GetPerceptionComponent()->ConfigureSense(*AiHearingConfig);

	GetPerceptionComponent()->SetDominantSense(*AiSenseConfig->GetSenseImplementation());
}

void AHAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	PawnAi = Cast<AHAi>(GetPawn());
	EnemyState = EEnemyState::Road;
	APlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	DelegateToLookingFor.BindUFunction(this,"TimerLookingFor",TimeInStateLookingFor);
	OwnUcharacterMovement = PawnAi->FindComponentByClass<UCharacterMovementComponent>();
}

void AHAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (EnemyState)
	{
	case EEnemyState::Detected:
		OwnUcharacterMovement->MaxWalkSpeed = EnemyRunSpeed;
		MoveToPlayer();
		break;
	case EEnemyState::Road:
		OwnUcharacterMovement->MaxWalkSpeed = EnemyWalkSpeed;
		if(EnemyState != EEnemyState::LookFor)
		{
			MoveToNextPoint();
		}
		break;
	case EEnemyState::HearSound:
		OwnUcharacterMovement->MaxWalkSpeed = EnemyRunSpeed;
		MoveToSound();
		break;
	case EEnemyState::LookFor:
		MoveToFind();
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
		if(Stimulus.Type == AiHearingConfig->GetSenseID())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Heard!"));
			EnemyState = EEnemyState::HearSound;
			SoundPosition = Stimulus.StimulusLocation;
		}

		if(Stimulus.Type == AiSenseConfig->GetSenseID())
		{
			const AHPlayer* Player = Cast<AHPlayer>(SensedActor);
			if(Player != nullptr)
			{
				if(Player->PlayerMovement != EPlayerMovement::Hide)
				{
					EnemyState = EEnemyState::Detected;
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Find!"));	
				}
			}
		}
	}else
	{
		const AHPlayer* Player = Cast<AHPlayer>(SensedActor);
		if(Player != nullptr && !Player->IsHide )
		{
			if(EnemyState != EEnemyState::LookFor)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerToLookingFor, DelegateToLookingFor,TimeInStateLookingFor,false);
			}
			EnemyState = EEnemyState::LookFor;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("LookFor!"));
		}
		
	}
	
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
	
	MoveToActor(APlayer);
}


void AHAI_Controller::ChangeRoad(FString RoadName)
{
	AHAi* HEnemy = Cast<AHAi>(GetPawn());
	if(HEnemy != nullptr && HEnemy->AllRoads[RoadName] != HEnemy->ActualRoad)
	{
		HEnemy->ActualRoad = HEnemy->AllRoads[RoadName];
	}
}

void AHAI_Controller::MoveToSound()
{
	MoveToLocation(SoundPosition);
	const FVector HaiPosition(PawnAi->GetActorLocation().X,PawnAi->GetActorLocation().Y,0);
	SoundPosition.Z = 0;
	if(HaiPosition.Equals(SoundPosition,100.f))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Is iN SOUND position!"));
		if(EnemyState != EEnemyState::LookFor)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerToLookingFor, DelegateToLookingFor,TimeInStateLookingFor,false);
		}
		EnemyState = EEnemyState::LookFor;
	}
}

void AHAI_Controller::MoveToFind()
{
	StopMovement();
}

void AHAI_Controller::TimerLookingFor(float LookforTime)
{
	if(EnemyState == EEnemyState::LookFor)
	{
		EnemyState = EEnemyState::Road;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("End look for!"));
}









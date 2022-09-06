// Fill out your copyright notice in the Description page of Project Settings.


#include "HAI_Controller.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "LaPorteProject/TimeObject/OpenClose.h"
#include "Perception/AISenseConfig_Hearing.h"


AHAI_Controller::AHAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception component")));
	AiSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sensor Config"));
	AiHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Component"));
	
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AHAI_Controller::OnPawnDetected);
	GetPerceptionComponent()->SetDominantSense(*AiSenseConfig->GetSenseImplementation());

	if(AiSenseConfig)
	{
		AILostSightRadius += AISightRadius;
		
		AiSenseConfig->SightRadius = AISightRadius;
		AiSenseConfig->LoseSightRadius = AILostSightRadius;
		AiSenseConfig->PeripheralVisionAngleDegrees = AISightFieldOfView;
		AiSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		AiSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AiSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
	}

	if(AiHearingConfig)
	{
		AiHearingConfig->HearingRange = AIHearingRadius;
		AiHearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		AiHearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AiHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		
	}
	GetPerceptionComponent()->ConfigureSense(*AiSenseConfig);
	GetPerceptionComponent()->ConfigureSense(*AiHearingConfig);
	
}

void AHAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	PawnAi = Cast<AHAi>(GetPawn());
	PawnAi->BoxCollisionHide->SetBoxExtent(FVector(AISightRadius,AISightRadius,64));
	EnemyState = EEnemyState::Road;
	APlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	DelegateToLookingFor.BindUFunction(this,"TimerLookingFor",TimeInStateLookingFor);
	DelegateToOpenDoor.BindUFunction(this,"TimerOpenDoor",TimerToOpenDoor);
	OwnUcharacterMovement = PawnAi->FindComponentByClass<UCharacterMovementComponent>();

	PawnAi->BoxCollisionHide->OnComponentBeginOverlap.AddDynamic(this,&AHAI_Controller::BeginOverlapHidingPlace);
	PawnAi->BoxCollisionHide->OnComponentEndOverlap.AddDynamic(this,&AHAI_Controller::EndOverlapHidingPlace);

	PawnMesh = PawnAi->GetMesh();
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
		LookFor();
		break;
	case EEnemyState::CheckAround:
		CheckAround();
		break;
	case EEnemyState::CheckHide:
		CheckHide();
		break;
	case EEnemyState::CheckInsideHidePlace:
		CheckInsideHide();
		break;
		
	}

	OpenDoor();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *UEnum::GetValueAsString(EnemyState));
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
		//Can hear player
		if(Stimulus.Type == AiHearingConfig->GetSenseID() && EnemyState != EEnemyState::Detected)
		{
			EnemyState = EEnemyState::HearSound;
			SoundPosition = Stimulus.StimulusLocation;
		}

		//Can see player
		if(Stimulus.Type == AiSenseConfig->GetSenseID())
		{
			const AHPlayer* Player = Cast<AHPlayer>(SensedActor);
			if(Player != nullptr)
			{
				//if player is not hide
				if(Player->PlayerMovement != EPlayerMovement::Hide)
				{
					EnemyState = EEnemyState::Detected;
				}else
				{
					//if player hide in front of enemy
					if(EnemyState == EEnemyState::Detected)
					{
						EnemyState = EEnemyState::LookFor;
					}else
					{
						
					}

				}
			}
		}
	}else
	{
		const AHPlayer* Player = Cast<AHPlayer>(SensedActor);
		if(Player != nullptr && !Player->IsHide)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerToLookingFor, DelegateToLookingFor,TimeInStateLookingFor,false);
			EnemyState = EEnemyState::CheckAround;
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
	const AHPlayer* Player = Cast<AHPlayer>(APlayer);
	if(Player)
	{
		if(Player->IsHide)
		{
			EnemyState = EEnemyState::LookFor;
		}
	}
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
	if(HaiPosition.Equals(SoundPosition,150.f))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Is iN SOUND position!"));
		EnemyState = EEnemyState::CheckAround;
		GetWorld()->GetTimerManager().SetTimer(TimerToLookingFor, DelegateToLookingFor,TimeInStateLookingFor,false);
	}
}

void AHAI_Controller::LookFor()
{
	//StopMovement();
	if(HidingPlaceToCheck.Num() > 0)
	{
		ObjectToCheck = HidingPlaceToCheck.Pop();
		EnemyState = EEnemyState::CheckHide;
	}else
	{
		//GetWorld()->GetTimerManager().SetTimer(TimerToLookingFor, DelegateToLookingFor,TimeInStateLookingFor,false);
		EnemyState = EEnemyState::Road;
	}
}

void AHAI_Controller::CheckHide()
{
	FVector PlaceToCheck = IOpenClose::Execute_GetPositionForInterraction(ObjectToCheck);
	MoveToLocation(PlaceToCheck);
	PlaceToCheck.Z = 0;
	const FVector HaiPosition(PawnAi->GetActorLocation().X,PawnAi->GetActorLocation().Y,0) ;
	if(HaiPosition.Equals(PlaceToCheck,50.f))
	{
		EnemyState = EEnemyState::CheckInsideHidePlace;
	}
}

void AHAI_Controller::CheckAround()
{
	StopMovement();
}

void AHAI_Controller::CheckInsideHide()
{
	StopMovement();
	if(ObjectToCheck != nullptr)
	{
		bool hasOpenClose = UKismetSystemLibrary::DoesImplementInterface(ObjectToCheck, UOpenClose::StaticClass());
		FVector PlaceToCheck = IOpenClose::Execute_GetPositionForInterraction(ObjectToCheck);
		PlaceToCheck.Z = 0;
		if(	hasOpenClose)
		{
			FRotator endRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(),PlaceToCheck);
			SetControlRotation(endRotation);
			IOpenClose::Execute_Open(ObjectToCheck);
			EnemyState = EEnemyState::CheckAround;
			GetWorld()->GetTimerManager().SetTimer(TimerToLookingFor, DelegateToLookingFor,TimeInStateLookingFor/2,false);

			AHPlayer* Player = Cast<AHPlayer>(APlayer);
			if(Player && Player->HideInObject == ObjectToCheck)
			{
				Player->Die();
			}
		}
	}
}

void AHAI_Controller::OpenDoor()
{
	if(CanOpenDoor)
	{
		FHitResult OutHit;
		FVector CamLoc;
		FRotator CamRot;

		const FVector StartTrace = PawnAi->GetActorLocation(); // trace start is the camera location
		FVector Direction = FVector::ZeroVector;
		if(PawnMesh)
		{
			Direction = PawnMesh->GetRightVector();
		}
	
		const FVector EndTrace = StartTrace + Direction * 100;

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		DrawDebugLine(GetWorld(),StartTrace,EndTrace, FColor::Green,false ,1,0,1);
		bool hitTrace = GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace,EndTrace, ECC_Visibility, TraceParams);
		if(hitTrace)
		{
			if(OutHit.Actor != nullptr && OutHit.Actor->Tags.Contains("Door"))
			{
				IOpenClose::Execute_OpenWithTransform(OutHit.GetActor(),PawnAi->GetActorLocation());
				StopMovement();
				CanOpenDoor =false;
				GetWorld()->GetTimerManager().SetTimer(TimerToOpenDoor,DelegateToOpenDoor,0.5f,false);
			}
		
		}
	}
	
}

void AHAI_Controller::BeginOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAInterractable* obj = Cast<AAInterractable>(OtherActor);
	
	if(obj != nullptr && obj->Tags.Contains("HidingPlace") && !HidingPlaceToCheck.Contains(obj))
	{
		HidingPlaceToCheck.AddUnique(obj);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Add Object!"));
	}
}

void AHAI_Controller::EndOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAInterractable* obj = Cast<AAInterractable>(OtherActor);

	if(obj != nullptr && obj->Tags.Contains("HidingPlace") && HidingPlaceToCheck.Contains(obj))
	{
		HidingPlaceToCheck.Remove(obj);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Delete Object!"));
	}
}

#pragma region Timer

void AHAI_Controller::TimerLookingFor(float LookforTime)
{
	if(EnemyState != EEnemyState::Detected)
	{
		EnemyState = EEnemyState::LookFor;
	}

}

void AHAI_Controller::TimerOpenDoor(float TimeToOpen)
{
	CanOpenDoor = true;
}

#pragma endregion Timer









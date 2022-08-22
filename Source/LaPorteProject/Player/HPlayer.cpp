// Fill out your copyright notice in the Description page of Project Settings.

#include "HPlayer.h"

#include "DrawDebugHelpers.h"
#include "HPlayer_Controller.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/GameSession.h"

#pragma region UE4_base
// Sets default values
AHPlayer::AHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OwnUcharacterMovement = this->FindComponentByClass<UCharacterMovementComponent>();
	
	//Instantiate player
	CameraLookAtWatchLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraLookAtWatchLocation"));
	CameraOriginLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraOriginLocation"));
	//CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	
	CurvedTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimeLinePlayer"));
	//CameraComp->bUsePawnControlRotation = true;


	CameraLookAtWatchLocation->SetupAttachment(GetMesh());
	CameraOriginLocation->SetupAttachment(GetMesh());
	//CameraComp->SetupAttachment(GetMesh(),"spine_03Socket");
	
}

// Called when the game starts or when spawned
void AHPlayer::BeginPlay()
{
	Super::BeginPlay();
	OwnUcharacterMovement->MaxWalkSpeed = SpeedWalk;
	ActualStamina = MaxStamina;

	HGameInstance = Cast<UHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(!HGameInstance)
	{
		UE_LOG(LogTemp,Fatal,TEXT("No game instance found"))
	}

	//Bind timeline
	if(CurveFloat)
	{
		//FOnTimelineFloat TimeLineProgress;
		//TimeLineProgress.BindUFunction(this,FName("TimeLineProgress"));
		UpdateFunctionFloatCameraTimeLine.BindDynamic(this, &AHPlayer::UpdateTimeLineCameraInProgress);
		
		CurvedTimeLine->SetTimelineLength(1.f);
		CurvedTimeLine->AddInterpFloat(CurveFloat,UpdateFunctionFloatCameraTimeLine);
	}
	
}

// Called every frame
void AHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageStamina(DeltaTime);
	//TargetObject(RangeInterraction);
	CurvedTimeLine->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);

	SetActorRelativeRotation(FRotator(0.f,Controller->GetControlRotation().Yaw, 0.f));
	
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

}

// Called to bind functionality to input
void AHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AHPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AHPlayer::LookUpAtRate);

	PlayerInputComponent->BindAction("Run", IE_Pressed,this, &AHPlayer::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AHPlayer::StopRun);

	PlayerInputComponent->BindAction("Time", IE_Pressed, this, &AHPlayer::StartTime);
	PlayerInputComponent->BindAction("LookWatch", IE_Pressed, this, &AHPlayer::LookWatch);

	PlayerInputComponent->BindAction("Interract",IE_Pressed, this, &AHPlayer::PickupObject);
	PlayerInputComponent->BindAction("Interract", IE_Released, this, &AHPlayer::RelaseAction);
}

#pragma endregion UE4_base

#pragma region Movement
void AHPlayer::MoveForward(float Value)
{
	if(PlayerMovement != EPlayerMovement::Hide && PlayerMovement != EPlayerMovement::Use)
	{
		const FRotator  Rotation = Controller->GetControlRotation();
		const FRotator  Direction(0.f,Rotation.Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(Direction).GetUnitAxis(EAxis::X);
		
		AddMovementInput(ForwardDirection,Value);
	}
	
}

void AHPlayer::MoveRight(float Value)
{
	if(PlayerMovement != EPlayerMovement::Hide && PlayerMovement != EPlayerMovement::Use)
	{
		const FRotator  Rotation = Controller->GetControlRotation();
		const FRotator  Direction(0.f,Rotation.Yaw, 0.f);
		const FVector RightDirection = FRotationMatrix(Direction).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection,Value);
	}
}

void AHPlayer::StartRun()
{
	if(PlayerMovement == EPlayerMovement::Walk && PlayerMovement != EPlayerMovement::Hide)
	{
		ChangeStateMovement(EPlayerMovement::Run);
	}

}

void AHPlayer::StopRun()
{
	if(	PlayerMovement == EPlayerMovement::Run)
	{
		ChangeStateMovement(EPlayerMovement::Walk);
	}
	
}

//Manage stamina
void AHPlayer::ManageStamina(float Delta)
{
	if(ActualStamina < 0 && PlayerMovement == EPlayerMovement::Run)
	{
		ChangeStateMovement(EPlayerMovement::Walk);
		GetWorldTimerManager().SetTimer(TimerStamina,this,&AHPlayer::CanRunNow,1.f,false,-1.f);
	}

	if(ActualStamina > MaxStamina)
	{
		ActualStamina = MaxStamina;
	}

	//Increase/Decrease stamina
	if(ActualStamina > 0 &&  PlayerMovement == EPlayerMovement::Run)
	{
		ActualStamina -= UseStaminaPerSeconds*Delta;
	}else
	{
		if(ActualStamina < MaxStamina && PlayerMovement != EPlayerMovement::Run && !GetWorldTimerManager().IsTimerActive(AHPlayer::TimerStamina))
		{
			ActualStamina += GainStaminaPerSeconds*Delta;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Stamina %f"),ActualStamina));	

}

void AHPlayer::CanRunNow()
{
	CanRun = true;
}

//State of player movement
void AHPlayer::ChangeStateMovement(const EPlayerMovement State)
{
	switch (State)
	{
	case EPlayerMovement::Run:
		OwnUcharacterMovement->MaxWalkSpeed = SpeedRun;
		break;
	case EPlayerMovement::Walk:
		OwnUcharacterMovement->MaxWalkSpeed = SpeedWalk;
		break;
	case EPlayerMovement::Watch:
		OwnUcharacterMovement->MaxWalkSpeed = 0;
		break;
	case EPlayerMovement::Hide:
		OwnUcharacterMovement->MaxWalkSpeed = 0;
		break;
	default: ;
	}

	PlayerMovement = State;
}

//Camera

void AHPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if(PlayerMovement != EPlayerMovement::Watch && PlayerMovement != EPlayerMovement::Use)
	{
		AddControllerYawInput(Rate  * GetWorld()->GetDeltaSeconds() * CameraRotationSpeed);
	}

}

void AHPlayer::LookUpAtRate(float Rate)
{
	TurnRate = Rate;
	// calculate delta for this frame from the rate information
	if(PlayerMovement != EPlayerMovement::Watch && PlayerMovement != EPlayerMovement::Use)
	{
		AddControllerPitchInput(Rate  * GetWorld()->GetDeltaSeconds() * CameraRotationSpeed);
	}
	
}


#pragma endregion Movement

#pragma region Action

//Input that change time in game (Day/night)
void AHPlayer::TimeChange(const ETimeInDay TimeToChange) const
{
	for( int32  i =0; i< HGameInstance->ObjectsAccordingTime.Num() ; ++i)
	{
		HGameInstance->ObjectsAccordingTime[i]->ChangeTransformAccordingTime(TimeToChange);
	}
}

//Input to look at your watch
void AHPlayer::LookWatch() 
{
	//CameraComp->SetRelativeLocation(FMath::Lerp(GetActorLocation(),CameraLookAtWatchLocation->GetComponentLocation(),0.1f));

	if(PlayerMovement != EPlayerMovement::Watch) //Look watch
	{
		CameraOriginLocation->SetWorldRotation(GetActorRotation());
		GoalTransformCamera = CameraLookAtWatchLocation->GetComponentTransform();
		CurvedTimeLine->PlayFromStart();
		ChangeStateMovement(EPlayerMovement::Watch);
	}else //Stop looking watch
	{
		CameraOriginLocation->SetWorldRotation(GoalTransformCamera.GetRotation());
		GoalTransformCamera = CameraOriginLocation->GetComponentTransform();
		CurvedTimeLine->PlayFromStart();
		ChangeStateMovement(EPlayerMovement::Walk);
	}
}

void AHPlayer::UpdateTimeLineCameraInProgress(float Value)
{

	//const float Angle = FMath::Acos(FVector::DotProduct(GetActorUpVector(), FVector::UpVector));
	//const FVector Axis = FVector::CrossProduct(GetActorForwardVector(), -FVector::UpVector).GetSafeNormal();
	//const FQuat DeltaRotation = FQuat(Axis, Angle);
	
	//GetController()->SetControlRotation(DeltaRotation.Rotator());
	//GetController()->ClientSetRotation(FRotator(FMath::Lerp(GetController()->GetControlRotation().Pitch,-40.f,Value),GetController()->GetControlRotation().Yaw,0));

	//GetController()->ClientSetRotation(FRotator(FMath::Lerp(GetController()->GetControlRotation().Pitch,-40.f,Value),GetController()->GetControlRotation().Yaw,0));
	//	GetController()->ClientSetRotation(FRotator(0.f,0.f, 0.f));
	//GetController()->ClientSetRotation(FRotator(-90,0,0));
	//CameraComp->SetWorldLocation(FMath::Lerp(CameraComp->GetComponentLocation(),GoalTransformCamera.GetLocation(),Value));
	//CameraComp->SetWorldRotation(FMath::Lerp(FQuat(CameraComp->GetComponentRotation()),FQuat(GoalTransformCamera.GetRotation()),Value));
	//Cast<AHPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->LookWatchRotation(Value,CameraOriginLocation->GetComponentRotation(),GoalTransformCamera.Rotator());

	/*const float Angle = FMath::Acos(FVector::DotProduct(GetActorUpVector(), FVector::UpVector));
	const FVector Axis = FVector::CrossProduct(GetActorUpVector(), FVector::UpVector).GetSafeNormal();
	const FQuat DeltaRotation = FQuat(Axis, Angle);
	AHPlayer_Controller *Player_Controller = Cast<AHPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CameraComp->AddRelativeRotation(DeltaRotation);*/
	
	//FRotator rot = FMath::Lerp(FQuat(CameraComp->GetComponentRotation()),FQuat(GoalTransformCamera.GetRotation()),Value).Rotator();
	//AHPlayer_Controller *Player_Controller = Cast<AHPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//Player_Controller->K2_SetActorRotation(rot,false);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Watch %f"),GetController()->GetControlRotation().Pitch));
}

//Raycast that check if there is an object which can be interract
FVector_NetQuantize AHPlayer::TargetObject(float Range)
{
	FHitResult OutHit;
	FVector CamLoc;
	FRotator CamRot;
	
	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
	const FVector StartTrace = CamLoc; // trace start is the camera location
	const FVector Direction = CamRot.Vector();
	
	const FVector EndTrace = StartTrace + Direction *Range;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(),StartTrace,EndTrace, FColor::Red,false ,1,0,1);
	bool hitTrace = GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace,EndTrace, ECC_Visibility, TraceParams);
	if(hitTrace)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Watch %s"),*OutHit.Actor->GetName()));
		return OutHit.Location;
	}

	return FVector_NetQuantize(EndTrace.X,EndTrace.Y,EndTrace.Z);
}

void AHPlayer::PickupObject()
{

	if(ObjectInHand == nullptr)
	{
		FHitResult OutHit;
		FVector CamLoc;
		FRotator CamRot;
	
		Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
		const FVector StartTrace = CamLoc; // trace start is the camera location
		const FVector Direction = CamRot.Vector();
	
		const FVector EndTrace = StartTrace + Direction * RangeInterraction;

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		DrawDebugLine(GetWorld(),StartTrace,EndTrace, FColor::Green,false ,1,0,1);
		bool hitTrace = GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace,EndTrace, ECC_Visibility, TraceParams);
		if(hitTrace)
		{
			IInterractable *InterractableObject = Cast<IInterractable>(OutHit.Actor);
			if(InterractableObject)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Watch %s"),*OutHit.Actor->GetName()));
				InterractableObject->Execute_Interract(InterractableObject->_getUObject());
			}
		}
		
	}else
	{
		ObjectInHand->Execute_StopInterract(ObjectInHand->_getUObject());
	}
}

void AHPlayer::RelaseAction()
{
	if(PlayerMovement == EPlayerMovement::Use)
	{
		ObjectInHand->Execute_StopInterract(ObjectInHand->_getUObject());
		PlayerMovement = EPlayerMovement::Walk;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("StopUse")));
	}
}

#pragma endregion Action


// Fill out your copyright notice in the Description page of Project Settings.

#include "HPlayer.h"

#include "Components/ArrowComponent.h"

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
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	CameraComp->bUsePawnControlRotation = true;


	CameraLookAtWatchLocation->SetupAttachment(GetMesh());
	CameraOriginLocation->SetupAttachment(GetMesh());
	CameraComp->SetupAttachment(GetMesh());
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
	
}

// Called every frame
void AHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageStamina(DeltaTime);
	TargetObject(RangeInterraction);
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

}

#pragma endregion UE4_base

#pragma region Movement
void AHPlayer::MoveForward(float Value)
{
	const FRotator  Rotation = Controller->GetControlRotation();
	const FRotator  Direction(0.f,Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(Direction).GetUnitAxis(EAxis::X);
	
	AddMovementInput(ForwardDirection,Value);
	
}

void AHPlayer::MoveRight(float Value)
{
	const FRotator  Rotation = Controller->GetControlRotation();
	const FRotator  Direction(0.f,Rotation.Yaw, 0.f);
	const FVector RightDirection = FRotationMatrix(Direction).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection,Value);
}

void AHPlayer::StartRun()
{
	ChangeStateMovement(EPlayerMovement::Run);
}

void AHPlayer::StopRun()
{
	ChangeStateMovement(EPlayerMovement::Walk);
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
	}

	PlayerMovement = State;
}

//Camera

void AHPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate  * GetWorld()->GetDeltaSeconds() * CameraRotationSpeed);
}

void AHPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate  * GetWorld()->GetDeltaSeconds() * CameraRotationSpeed);
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
	//UCameraComponent PlayerCamera = GetCompo
	
}

//Raycast that check if there is an object which can be interract
void AHPlayer::TargetObject(float Range)
{
	FVector CamLoc;
	FRotator CamRot;
	
	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
	const FVector StartTrace = CamLoc; // trace start is the camera location
	const FVector Direction = CamRot.Vector();
	
	const FVector EndTrace = StartTrace + Direction *Range;

	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")),true,this);

}


#pragma endregion Action


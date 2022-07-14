// Fill out your copyright notice in the Description page of Project Settings.

#include "HPlayer.h"

#pragma region UE4_base
// Sets default values
AHPlayer::AHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OwnUcharacterMovement = this->FindComponentByClass<UCharacterMovementComponent>();
}

// Called when the game starts or when spawned
void AHPlayer::BeginPlay()
{
	Super::BeginPlay();
	OwnUcharacterMovement->MaxWalkSpeed = SpeedWalk;
	ActualStamina = MaxStamina;
}

// Called every frame
void AHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageStamina(DeltaTime);
}

// Called to bind functionality to input
void AHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AHPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AHPlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Run", IE_Pressed,this, &AHPlayer::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AHPlayer::StopRun);

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

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Stamina %f"),ActualStamina));	

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


#pragma endregion Movement



// Fill out your copyright notice in the Description page of Project Settings.

#include "HPlayer.h"

// Sets default values
AHPlayer::AHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AHPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AHPlayer::AddControllerPitchInput);

}

void AHPlayer::MoveForward(float Value)
{
	const FRotator  Rotation = Controller->GetControlRotation();
	const FRotator  Direction(0.f,Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(Direction).GetUnitAxis(EAxis::X);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%f"),speedRunning));
	AddMovementInput(ForwardDirection,Value);
}

void AHPlayer::MoveRight(float Value)
{
	const FRotator  Rotation = Controller->GetControlRotation();
	const FRotator  Direction(0.f,Rotation.Yaw, 0.f);
	const FVector RightDirection = FRotationMatrix(Direction).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection,Value);
}


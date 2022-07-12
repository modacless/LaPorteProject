// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHPlayer.h"

// Sets default values
AFPHPlayer::AFPHPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPHPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPHPlayer::MoveForward(float Value)
{
}

void AFPHPlayer::MoveRight(float Value)
{
}


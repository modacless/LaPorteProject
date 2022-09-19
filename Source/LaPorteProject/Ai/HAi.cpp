// Fill out your copyright notice in the Description page of Project Settings.


#include "HAi.h"

#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AHAi::AHAi()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	BoxCollisionHide = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision check hide place"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,500.0f,0.0f);
	
}

// Called when the game starts or when spawned
void AHAi::BeginPlay()
{
	Super::BeginPlay();
	BoxCollisionHide->OnComponentBeginOverlap.AddDynamic(this,&AHAi::BeginOverlapHidingPlace);
	BoxCollisionHide->OnComponentEndOverlap.AddDynamic(this,&AHAi::EndOverlapHidingPlace);
	
}

// Called every frame
void AHAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHAi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHAi::ChangeToNextRoad()
{
	if(ActualRoad != nullptr)
	{
		if(ActualRoad->NextRoad != nullptr)
		{
			ActualRoad = ActualRoad->NextRoad;
		}
	}
}

void AHAi::BeginOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
											  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAInterractable* obj = Cast<AAInterractable>(OtherActor);
	AHAI_Controller *PawnController = Cast<AHAI_Controller>(GetController());
	if(obj != nullptr && obj->Tags.Contains("HidingPlace") && !HidingPlaceToCheck.Contains(obj))
	{
		HidingPlaceToCheck.AddUnique(obj);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Add Object!"));
	}
}

void AHAi::EndOverlapHidingPlace(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAInterractable* obj = Cast<AAInterractable>(OtherActor);

	if(obj != nullptr && obj->Tags.Contains("HidingPlace") && HidingPlaceToCheck.Contains(obj))
	{
		HidingPlaceToCheck.Remove(obj);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Delete Object!"));
	}
}



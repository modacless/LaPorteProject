// Copyright Epic Games, Inc. All Rights Reserved.

#include "LaPorteProjectGameMode.h"
#include "LaPorteProjectHUD.h"
#include "LaPorteProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALaPorteProjectGameMode::ALaPorteProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALaPorteProjectHUD::StaticClass();
}

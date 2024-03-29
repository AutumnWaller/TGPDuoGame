// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TGPDuoGameMode.h"
#include "TGPDuoHUD.h"
#include "TGPDuoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATGPDuoGameMode::ATGPDuoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATGPDuoHUD::StaticClass();
}

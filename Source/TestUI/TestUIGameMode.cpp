// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestUIGameMode.h"
#include "TestUICharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestUIGameMode::ATestUIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

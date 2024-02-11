// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPGameMode.h"
#include "GSPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGSPGameMode::AGSPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

void AGSPHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_MappingContext, 0);
		}
	}
}

UEnhancedInputComponent* AGSPHeroCharacter::GetEnhancedInputComponent() const
{
	return Cast<UEnhancedInputComponent>(GetController()->InputComponent);
}

void AGSPHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
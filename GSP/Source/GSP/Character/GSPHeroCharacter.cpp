// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "../Input/GSPInputConfig.h"
#include "GSP/Ability/GSPAbilitySystemComponent.h"

void AGSPHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if(_MappingContext)
	{
		if (const APlayerController* PC = Cast<APlayerController>(Controller))
		{
			if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(_MappingContext, 0);
				UE_LOG(GSPCharacter, Warning, TEXT("Added Player Mapping Context"))

			}

			if(const auto inputComp = Cast<UEnhancedInputComponent>(InputComponent))
			{
				UE_LOG(GSPCharacter, Warning, TEXT("Found Player Input Component"))

				if (const UInputAction* iaAbilityOne = _InputConfig->FindAbilityInputActionForTag(FGameplayTag::RequestGameplayTag("InputTag.Ability.One")))
				{

					inputComp->BindAction(iaAbilityOne, ETriggerEvent::Completed, this, &AGSPHeroCharacter::AbilityOne);
					UE_LOG(GSPCharacter, Warning, TEXT("Bound Ability One Input Action"))
				}
				else
				{
					UE_LOG(GSPCharacter, Error, TEXT("Failed to get Ability One Input Action"))
				}

				if (const UInputAction* iaAbilityTwo = _InputConfig->FindAbilityInputActionForTag(FGameplayTag::RequestGameplayTag("InputTag.Ability.Two")))
				{

					inputComp->BindAction(iaAbilityTwo, ETriggerEvent::Completed, this, &AGSPHeroCharacter::AbilityTwo);
					UE_LOG(GSPCharacter, Warning, TEXT("Bound Ability Two Input Action"))
				}
				else
				{
					UE_LOG(GSPCharacter, Error, TEXT("Failed to get Ability Two Input Action"))

				}
			}
			else
			{
				UE_LOG(GSPCharacter, Error, TEXT("Failed to get inputComp"))
			}
		}
	}
}

AGSPHeroCharacter::AGSPHeroCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	_AbilityOne = FGameplayTag::RequestGameplayTag("Game.Element.Ability.FireAttack");
	_AbilityTwo = FGameplayTag::RequestGameplayTag("Game.Element.Ability.RockArmour");

}

UEnhancedInputComponent* AGSPHeroCharacter::GetEnhancedInputComponent() const
{
	return Cast<UEnhancedInputComponent>(GetController()->InputComponent);
}

float AGSPHeroCharacter::ActivateAbillityDuration(const FGameplayTag& AbilityTag)
{
	if (_AbilitySystemComponent)
	{
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(AbilityTag);
		if (_AbilitySystemComponent->TryActivateAbilitiesByTag(tagContainer))
		{
			FGameplayEffectQuery query; query.EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchAnyTags(tagContainer);

			if (TArray<float> durations = _AbilitySystemComponent->GetActiveEffectsDuration(query); durations.Num() > 0)
			{
				UE_LOG(GSPCharacter, Warning, TEXT("Ability Duration: %f"), durations[0])
					return durations[0];
			}
		}
	}

	return 0.0f;
}

void AGSPHeroCharacter::AbilityOne()
{
	UE_LOG(GSPCharacter, Warning, TEXT("Ability One"))

	if (float duration = ActivateAbillityDuration(_AbilityOne); duration > 0.0f)
	{
		OnAbilityOne(duration);
	}
	
}

void AGSPHeroCharacter::AbilityTwo()
{
	UE_LOG(GSPCharacter, Warning, TEXT("Ability Two"))
		
	if (float duration = ActivateAbillityDuration(_AbilityTwo); duration > 0.0f)
	{
		OnAbilityTwo(duration);
	}
}

void AGSPHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

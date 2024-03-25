// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPPlayerState.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "../Character/GSPCharacter.h"

AGSPPlayerState::AGSPPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	_AbilitySystemComponent = CreateDefaultSubobject<UGSPAbilitySystemComponent>(TEXT("_AbilitySystem")); //Component
	_AbilitySystemComponent->SetIsReplicated(true);
	
	// Mixed mode means we only are replicated the GEs to ourself
	_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Create the attribute set, this replicates by default
	// automatically registers the AttributeSet with the _AbilitySystemComponent
	_AttributeSet = CreateDefaultSubobject<UGSPAttributeSet>(TEXT("AttributeSet"));
	
	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	NetUpdateFrequency = 100.0f;
	
	_DeadTag = FGameplayTag::RequestGameplayTag("Actor.State.Dead");
}

UAbilitySystemComponent* AGSPPlayerState::GetAbilitySystemComponent() const
{
	return _AbilitySystemComponent;
}

UGSPAttributeSet* AGSPPlayerState::GetGSPAttributeSet() const
{
	return _AttributeSet;
}

bool AGSPPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AGSPPlayerState::GetHealth() const
{
	return _AttributeSet->GetHealth();
}

float AGSPPlayerState::GetMaxHealth() const
{
	return _AttributeSet->GetMaxHealth();
}

float AGSPPlayerState::GetHealthRegenRate() const
{
	return _AttributeSet->GetHealthRegenRate();
}


float AGSPPlayerState::GetStamina() const
{
	return _AttributeSet->GetStamina();
}

float AGSPPlayerState::GetMaxStamina() const
{
	return _AttributeSet->GetMaxStamina();
}

float AGSPPlayerState::GetStaminaRegenRate() const
{
	return _AttributeSet->GetStaminaRegenRate();
}

float AGSPPlayerState::GetShield() const
{
	return _AttributeSet->GetShield();
}

float AGSPPlayerState::GetMaxShield() const
{
	return _AttributeSet->GetMaxShield();
}

float AGSPPlayerState::GetShieldRegenRate() const
{
	return _AttributeSet->GetShieldRegenRate();
}

float AGSPPlayerState::GetArmor() const
{
	return _AttributeSet->GetArmor();
}

float AGSPPlayerState::GetMoveSpeed() const
{
	return _AttributeSet->GetMoveSpeed();
}

int32 AGSPPlayerState::GetCharacterLevel() const
{
	return _AttributeSet->GetCharacterLevel();
}

int32 AGSPPlayerState::GetXP() const
{
	return _AttributeSet->GetXP();
}

void AGSPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (_AbilitySystemComponent)
	{
		// Health change callbacks
		HealthChangedDelegateHandle = _AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(_AttributeSet->GetHealthAttribute()).AddUObject(this, &AGSPPlayerState::HealthChanged);
	}
}

void AGSPPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	// Check for and handle death
	AGSPCharacter* Hero = Cast<AGSPCharacter>(GetPawn());
	if (IsValid(Hero) && !IsAlive() && !_AbilitySystemComponent->HasMatchingGameplayTag(_DeadTag))
	{
		if (Hero)
		{
			Hero->Death();
			UE_LOG(LogTemp, Warning, TEXT("AGSPPlayerState::HealthChanged: %s is dead."), *Hero->GetName());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("HealthChanged: %f"), Data.NewValue);
}
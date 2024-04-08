// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/GSPDestructibleObject.h"
#include "AbilitySystemComponent.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AGSPDestructibleObject::AGSPDestructibleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create ability system component
	_AbilitySystemComponent = CreateDefaultSubobject<UGSPAbilitySystemComponent>(TEXT("_AbilitySystem")); //Component
	_AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	// automatically registers the AttributeSet with the _AbilitySystemComponent
	_AttributeSet = CreateDefaultSubobject<UGSPAttributeSet>(TEXT("AttributeSet"));

}


//handles death of object
void AGSPDestructibleObject::Death() 
{
	OnDeath();
}
void AGSPDestructibleObject::FinishDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Health on death: %f"), GetHealth());
	Destroy();
}

bool AGSPDestructibleObject::IsAlive() const
{
	return GetHealth() > 0.0f;
}

//getters for attributes
float AGSPDestructibleObject::GetHealth() const
{
	return _AttributeSet->GetHealth();
}

float AGSPDestructibleObject::GetMaxHealth() const
{
	return _AttributeSet->GetMaxHealth();
}

float AGSPDestructibleObject::GetHealthRegenRate() const
{
	return _AttributeSet->GetHealthRegenRate();
}

//getter for ability system component
UAbilitySystemComponent* AGSPDestructibleObject::GetAbilitySystemComponent() const
{
	return(UAbilitySystemComponent*)_AbilitySystemComponent;
}


//getter for attribute set
UGSPAttributeSet* AGSPDestructibleObject::GetGSPAttributeSet() const
{
	return _AttributeSet;
}


// Called when the game starts or when spawned
void AGSPDestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	if (_NiagaraSpawnFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _NiagaraSpawnFX, GetActorLocation());
	}

	if (_AbilitySystemComponent)
	{
		// Health change callbacks
		//HealthChangedHandler = _AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(_AttributeSet->GetHealthAttribute()).AddUObject(this, &AGSPDestructibleObject::HealthChanged);
	}
	_AttributeSet->SetMaxHealth(200.0f);
	_AttributeSet->SetHealth(200.0f);
}

//handles health changing and death
//void AGSPDestructibleObject::HealthChanged(const FOnAttributeChangeData& Data)


void AGSPDestructibleObject::CheckDeath() const
{
	if (!IsAlive())
	{
		OnDeath();
	}


	return;
}

// Called every frame
void AGSPDestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}





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

// Called when the game starts or when spawned
void AGSPDestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	if (_NiagaraSpawnFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _NiagaraSpawnFX, GetActorLocation());
	}
	
}


UAbilitySystemComponent* AGSPDestructibleObject::GetAbilitySystemComponent() const
{
	return(UAbilitySystemComponent*) _AbilitySystemComponent;
}

// Called every frame
void AGSPDestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UGSPAttributeSet* AGSPDestructibleObject::GetGSPAttributeSet() const
{
	return _AttributeSet;
}



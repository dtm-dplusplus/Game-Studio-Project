// Fill out your copyright notice in the Description page of Project Settings.


#include "../Character/GSPDestructibleObject.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AGSPDestructibleObject::AGSPDestructibleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called every frame
void AGSPDestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UGSPAttributeSet* AGSPDestructibleObject::GetGSPAttributeSet() const
{
	return _AttributeSet;
}



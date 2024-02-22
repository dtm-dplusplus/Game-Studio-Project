// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPAbilitySystemComponent.h"

#include "GSPGlobalAbilitySystem.h"

DEFINE_LOG_CATEGORY(GSPAbility)

UGSPAbilitySystemComponent::UGSPAbilitySystemComponent()
{

}

void UGSPAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UGSPGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGSPGlobalAbilitySystem>(GetWorld()))
	{
		GlobalAbilitySystem->AddASC(this);
	}
}

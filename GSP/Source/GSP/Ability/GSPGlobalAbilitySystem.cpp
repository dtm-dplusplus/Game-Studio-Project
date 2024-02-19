// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPGlobalAbilitySystem.h"
#include "GSPAbilitySystemComponent.h"

UGSPGlobalAbilitySystem::UGSPGlobalAbilitySystem()
{
}

void UGSPGlobalAbilitySystem::AddASC(UGSPAbilitySystemComponent* ASC)
{
	check(ASC);

	// Apply Global Effects/Abilities
	//....

	ASCs.AddUnique(ASC);
}

void UGSPGlobalAbilitySystem::RemoveASC(UGSPAbilitySystemComponent* ASC)
{
	check(ASC);

	// Remove Global Effects/Abilities
	// ....

	ASCs.Remove(ASC);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPGlobalAbilitySystem.h"
#include "GSPAbilitySystemComponent.h"

UGSPGlobalAbilitySystem::UGSPGlobalAbilitySystem()
{
}

void UGSPGlobalAbilitySystem::ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability)
{
	for (UGSPAbilitySystemComponent* ASC : _ASCs)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0));
	}
}

void UGSPGlobalAbilitySystem::ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect)
{
}

void UGSPGlobalAbilitySystem::AddASC(UGSPAbilitySystemComponent* ASC)
{
	check(ASC);

	// Apply Global Effects/Abilities
	//....

	_ASCs.AddUnique(ASC);
}

void UGSPGlobalAbilitySystem::RemoveASC(UGSPAbilitySystemComponent* ASC)
{
	check(ASC);

	// Remove Global Effects/Abilities
	// ....

	_ASCs.Remove(ASC);
}

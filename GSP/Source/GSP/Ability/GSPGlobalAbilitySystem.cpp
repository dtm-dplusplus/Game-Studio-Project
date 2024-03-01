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

	UE_LOG(LogTemp, Log, TEXT("Added ASC %s to Global Ability System"), *ASC->GetName());
}

void UGSPGlobalAbilitySystem::RemoveASC(UGSPAbilitySystemComponent* ASC)
{
	check(ASC);
	
	// ....

	UE_LOG(LogTemp, Log, TEXT("Removed ASC %s to Global Ability System"), *ASC->GetName());
	_ASCs.Remove(ASC);

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GSPGlobalAbilitySystem.generated.h"

class UGSPAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GSP_API UGSPGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UGSPGlobalAbilitySystem();

	/** Register an ASC with global system and apply any active global effects/abilities. */
	void AddASC(UGSPAbilitySystemComponent* ASC);

	/** Removes an ASC from the global system, along with any active global effects/abilities. */
	void RemoveASC(UGSPAbilitySystemComponent* ASC);

private:

	/** Ability System Compents added by their owning pawns */
	UPROPERTY()
	TArray<TObjectPtr<UGSPAbilitySystemComponent>> ASCs;
};

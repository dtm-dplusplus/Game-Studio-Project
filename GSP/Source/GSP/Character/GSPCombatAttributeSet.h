// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "GSPCombatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GSP_API UGSPCombatAttributeSet : public UGSPAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UGSPCombatAttributeSet, _AttackStrength)
	ATTRIBUTE_ACCESSORS(UGSPCombatAttributeSet, _DefendStrength)
	ATTRIBUTE_ACCESSORS(UGSPCombatAttributeSet, _SupportStrength)

private:
	//~ Begin UAttributeSet interface
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~ End UAttributeSet interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _AttackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _DefendStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _SupportStrength;
};

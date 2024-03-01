// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability/GSPAttributeSet.h"
#include "GSPCharacterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GSP_API UGSPCharacterAttributeSet : public UGSPAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UGSPCharacterAttributeSet, _Health)
	ATTRIBUTE_ACCESSORS(UGSPCharacterAttributeSet, _MaxHealth)
	ATTRIBUTE_ACCESSORS(UGSPCharacterAttributeSet, _SupportStrength)

private:
	//~ Begin UAttributeSet interface
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~ End UAttributeSet interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _SupportStrength;
};

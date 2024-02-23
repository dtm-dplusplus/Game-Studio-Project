// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GSPAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GSP_API UGSPAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Character Health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Character")
	FGameplayAttributeData _Health;

	GAMEPLAYATTRIBUTE_VALUE_GETTER(_Health);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(_Health)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(_Health);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UGSPAttributeSet, _Health);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Character")
	FGameplayAttributeData _MaxHealth;

	GAMEPLAYATTRIBUTE_VALUE_GETTER(_MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(_MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(_MaxHealth);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UGSPAttributeSet, _MaxHealth);
};



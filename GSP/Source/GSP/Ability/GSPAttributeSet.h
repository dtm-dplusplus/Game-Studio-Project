// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GSPAttributeSet.generated.h"

// Uses macros from _AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Delegate used to broadcast attribute events.
//DECLARE_MULTICAST_DELEGATE_FourParams(FGSPAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGSPAttributeEvent, float, NewValuefloat, float, NewValueNorm);


UCLASS()
class GSP_API UGSPAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// Current Health, when 0 we expect owner to die unless prevented by an ability. Capped by MaxHealth.
	// Positive changes to Health should go through Healing meta attribute.
	// Negative changes to Health should go through Damage meta attribute.
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, Health)

	UFUNCTION(BlueprintCallable)
	float GetHealthNormalized() const;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FGSPAttributeEvent HealthChangeDelegate;

	// MaxHealth is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, MaxHealth)

	// Health regen rate will passively increase Health every second
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, HealthRegenRate)

	// Current shield acts like temporary health. When depleted, damage will drain regular health.
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, Shield)

	UPROPERTY(BlueprintAssignable, Category = "Shield")
	FGSPAttributeEvent ShieldChangeDelegate;

	// Maximum shield that we can have.
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, MaxShield)

	UFUNCTION(BlueprintCallable)
	float GetShieldNormalized() const;

	// Meta Attributes below - These are not stateful and exist temporarily //

	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, Damage)

	// Healing is a meta attribute into + Health
	UPROPERTY(BlueprintReadOnly, Category = "Healing")
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, Healing)

	//Shield regen rate will passively increase Shield every second
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	FGameplayAttributeData ShieldRepair;
	ATTRIBUTE_ACCESSORS(UGSPAttributeSet, ShieldRepair)
};
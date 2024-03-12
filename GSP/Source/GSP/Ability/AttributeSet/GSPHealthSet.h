// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "../GSPAttributeSet.h"
#include "GSPHealthSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GSP_API UGSPHealthSet : public UGSPAttributeSet
{
	GENERATED_BODY()

public:
	UGSPHealthSet();

	ATTRIBUTE_ACCESSORS(UGSPHealthSet, _Health);
	ATTRIBUTE_ACCESSORS(UGSPHealthSet, _MaxHealth);
	ATTRIBUTE_ACCESSORS(UGSPHealthSet, _Healing);
	ATTRIBUTE_ACCESSORS(UGSPHealthSet, _Damage);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FGSPAttributeEvent OnOutOfHealth;

private:
	// The current health attribute.  The health will be capped by the max health attribute.
	UPROPERTY(BlueprintReadOnly, Category = "GSP|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "GSP|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _MaxHealth;

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Temporay attributes from effects
private:
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category = "GSP|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category = "GSP|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _Damage;
};

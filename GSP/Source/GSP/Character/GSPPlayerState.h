// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "GSPPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGSPOnAttributeValueChanged, FGameplayAttribute, Attribute, float,
                                               NewValue, float, OldValue);
struct FOnAttributeChangeData;
class UGSPAttributeSet;

/**
 * Player State for GSP Characters
 */
UCLASS()
class GSP_API AGSPPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGSPPlayerState();

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End IAbilitySystemInterface */

	UGSPAttributeSet* GetGSPAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState")
	bool IsAlive() const;

	/** Getters for attributes from GSPAttributeSetBase. Returns Current Value unless otherwise specified. */

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetMaxShield() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetShieldRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetArmor() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|PlayerState|Attributes")
	int32 GetXP() const;

protected:
	FGameplayTag _DeadTag;

	UPROPERTY(EditAnywhere, Category = "GSP|Abilities")
	class UGSPAbilitySystemComponent* _AbilitySystemComponent;

	UPROPERTY()
	UGSPAttributeSet* _AttributeSet;

	// Attribute changed delegate handles
	FDelegateHandle HealthChangedDelegateHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};

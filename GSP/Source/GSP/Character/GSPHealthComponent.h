// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/AttributeSet/GSPHealthSet.h"
#include "Components/ActorComponent.h"
#include "GSPHealthComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GSPHealthComponent, Log, All);

class UGSPHealthSet;
class UGSPAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGSPHealth_AttributeChanged, UGSPHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent) )
class GSP_API UGSPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGSPHealthComponent();

public:
	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "GSP|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "GSP|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0]. Useful for UI.
	UFUNCTION(BlueprintCallable, Category = "GSP|Health")
	float GetHealthNormalized() const;

	void Initialize(UGSPAbilitySystemComponent* ASC);

public:
	/** Delegate for when the health attribute changes. */
	UPROPERTY(BlueprintAssignable, Category = "GSP|Health")
	FGSPHealth_AttributeChanged _OnHealthChanged;

protected:
	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UGSPAbilitySystemComponent> _AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UGSPHealthSet> _HealthSet;
};
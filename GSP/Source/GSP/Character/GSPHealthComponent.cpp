// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPHealthComponent.h"
#include "GameplayEffectExtension.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "../Ability/AttributeSet/GSPHealthSet.h"

DEFINE_LOG_CATEGORY(GSPHealthComponent)
UGSPHealthComponent::UGSPHealthComponent()
{
	// These are set in Initialize
	_AbilitySystemComponent = nullptr;
	_HealthSet = nullptr;
}

float UGSPHealthComponent::GetHealth() const
{
	return _HealthSet->Get_Health();
}

float UGSPHealthComponent::GetMaxHealth() const
{
	return _HealthSet->Get_MaxHealth();
}

float UGSPHealthComponent::GetHealthNormalized() const
{
	return  _HealthSet->Get_Health() / _HealthSet->Get_MaxHealth();
}

void UGSPHealthComponent::Initialize(UGSPAbilitySystemComponent* ASC)
{
	const AActor* Owner = GetOwner();
	check(Owner);

	if (_AbilitySystemComponent)
	{
		UE_LOG(GSPHealthComponent, Error, TEXT("GSPHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	_AbilitySystemComponent = ASC;
	if (!_AbilitySystemComponent)
	{
		UE_LOG(GSPHealthComponent, Error, TEXT("GSPHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	_HealthSet = _AbilitySystemComponent->GetSet<UGSPHealthSet>();
	if (!_HealthSet)
	{
		UE_LOG(GSPHealthComponent, Error, TEXT("GSPHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// Register to listen for attribute changes.
	_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGSPHealthSet::Get_HealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);

	// Set attributes to default values.
	_AbilitySystemComponent->SetNumericAttributeBase(UGSPHealthSet::Get_HealthAttribute(), _HealthSet->Get_MaxHealth());

	// Broadcast the initial health value. This is useful for UI.
	_OnHealthChanged.Broadcast(this, _HealthSet->Get_Health(), _HealthSet->Get_Health(), nullptr);
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

	return nullptr;
}

void UGSPHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	_OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

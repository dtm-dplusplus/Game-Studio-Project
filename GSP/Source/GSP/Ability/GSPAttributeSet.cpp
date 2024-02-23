// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

void UGSPAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == Get_HealthAttribute())
	{
		// Clamp Health
		_Health.SetBaseValue(FMath::Clamp(_Health.GetCurrentValue(), 0.0f, _MaxHealth.GetCurrentValue()));
	}
}
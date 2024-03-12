// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPHealthSet.h"
#include "AbilitySystemComponent.h"

#include "GameplayEffectExtension.h"

UGSPHealthSet::UGSPHealthSet(): _Health{100.f}, _MaxHealth{ 100.f }
{
	bOutOfHealth = false;
}

void UGSPHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	UE_LOG(LogTemp, Warning, TEXT("UGSPHealthSet::PostGameplayEffectExecute"));
	//const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TAG_Gameplay_DamageSelfDestruct);
	float MinimumHealth = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Test Log"))
	UE_LOG(LogTemp, Warning, TEXT("Get_MaxHealth: [%f]"), Get_MaxHealth())
	UE_LOG(LogTemp, Warning, TEXT("Get_Healing: [%f]"), Get_Healing())
	UE_LOG(LogTemp, Warning, TEXT("Get_Damage: [%f]"), Get_Damage())
	UE_LOG(LogTemp, Warning, TEXT("Get_Health: [%f]"), Get_Health())
	UE_LOG(LogTemp, Warning, TEXT("Artificial new health: [%f]"), Get_Health() - Get_Damage())
	//Set_Damage(0);

	// Equivelant to Set_Damage
	{
		
		UAbilitySystemComponent* AbilityComp = Cast<UAbilitySystemComponent>(GetOwningActor());
		if (ensure(AbilityComp))
		{
			AbilityComp->SetNumericAttributeBase(Get_DamageAttribute(), 0);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("New Get_Damage: [%f]"), Get_Damage())

	if (Data.EvaluatedData.Attribute == Get_DamageAttribute())
	{
		//Set_Health(Get_Health() - Get_Damage());

		// Convert into -Health and then clamp
		//UE_LOG(LogTemp, Warning, TEXT("Health: [%f] Damage: [%f], Owner [%s]"), Get_Health(), Get_Damage(), *GetNameSafe(GetOwningActor()))
		
		//Set_Health(FMath::Clamp(Get_Health() - Get_Damage(), MinimumHealth, Get_MaxHealth()));
		//Set_Damage(0.0f);
	}
	//else if (Data.EvaluatedData.Attribute == Get_HealingAttribute())
	//{
	//	// Convert into +Health and then clamo
	//	UE_LOG(LogTemp, Warning, TEXT("Health: [%f] Healing: [%f], Owner [%s]"), Get_Health(), Get_Healing(), *GetNameSafe(GetOwningActor()))

	//	//Set_Health(FMath::Clamp(Get_Health() + Get_Healing(), MinimumHealth, Get_MaxHealth()));
	//	Set_Healing(0.0f);
	//}
	//else if (Data.EvaluatedData.Attribute == Get_HealthAttribute())
	//{
	//	// Clamp and fall into out of health handling below
	//	UE_LOG(LogTemp, Warning, TEXT("Set Health: [%f], Owner [%s]"), Get_Health(), *GetNameSafe(GetOwningActor()))

	//	//Set_Health(FMath::Clamp(Get_Health(), MinimumHealth, Get_MaxHealth()));
	//}

	//if ((Get_Health() <= 0.0f) && !bOutOfHealth)
	//{
	//	if (OnOutOfHealth.IsBound())
	//	{
	//		const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	//		AActor* Instigator = EffectContext.GetOriginalInstigator();
	//		AActor* Causer = EffectContext.GetEffectCauser();

	//		bOutOfHealth = true;
	//		OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
	//	}
	//}

	// Check health again in case an event above changed it. Disabled for now
	//bOutOfHealth = (Get_Health() <= 0.0f);
}

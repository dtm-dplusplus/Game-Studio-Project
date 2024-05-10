// Fill out your copyright notice in the Description page of Project Settings.

#include "GSPAttributeSet.h"

#include "../Character/GSPCharacter.h"
#include "../Character/GSPDestructibleObject.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
//#include "InputCore.h"

float UGSPAttributeSet::GetHealthNormalized() const
{
	return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
}

float UGSPAttributeSet::GetShieldNormalized() const
{
	return Shield.GetCurrentValue() / MaxShield.GetCurrentValue();
}

void UGSPAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AGSPCharacter* TargetCharacter = nullptr;
	AGSPDestructibleObject* TargetObject = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		if (TargetController)
		{
			TargetCharacter = Cast<AGSPCharacter>(TargetActor);
		}
		else
		{
			TargetObject = Cast<AGSPDestructibleObject>(TargetActor);
		}
		
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AGSPCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<AGSPCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AGSPCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = true;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}
			if (TargetCharacter)
			{
				if (!TargetCharacter->IsAlive())
				{
					UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), *FString(__FUNCTION__), *TargetCharacter->GetName());
				}
			}
			

			// Apply the damage to shield first if it exists
			const float OldShield = GetShield();
			float DamageAfterShield = LocalDamageDone - OldShield;
			if (OldShield > 0)
			{
				float NewShield = OldShield - LocalDamageDone;
				SetShield(FMath::Clamp<float>(NewShield, 0.0f, GetMaxShield()));
				ShieldChangeDelegate.Broadcast(GetShield(), GetShieldNormalized());
			}

			// If there is still damage left after shield, apply damage
			if (DamageAfterShield > 0)
			{
				// Apply the health change and then clamp it
				const float NewHealth = GetHealth() - DamageAfterShield;
				SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
				HealthChangeDelegate.Broadcast(GetHealth(), GetHealthNormalized());
				UE_LOG(LogTemp, Log, TEXT("%s() %s Health: %f"), *FString(__FUNCTION__), *GetOwningActor()->GetName(), NewHealth);
			}

			if (TargetCharacter && WasAlive)
			{
				// This is the log statement for damage received. Turned off for live games.
				UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), *FString(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Handle healing.
		const float LocalHealing = GetHealing();
		SetHealing(0.f);

		SetHealth(FMath::Clamp(GetHealth() + LocalHealing, 0.0f, GetMaxHealth()));
		HealthChangeDelegate.Broadcast(GetHealth(), GetHealthNormalized());
	}
	else if (Data.EvaluatedData.Attribute == GetShieldRepairAttribute())
	{
		// Clear shield repair value
		float LocalShieldRepair = GetShieldRepair();
		SetShieldRepair(0.0f);
		SetShield(FMath::Clamp(GetShield() + LocalShieldRepair, 0.0f, GetMaxShield()));
		ShieldChangeDelegate.Broadcast(GetShield(), GetShieldNormalized());
		UE_LOG(LogTemp, Log, TEXT("%s() %s Shield Repair: %f"), *FString(__FUNCTION__), *GetOwningActor()->GetName(), LocalShieldRepair);
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		// Handle shield changes.
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
		ShieldChangeDelegate.Broadcast(GetShield(), GetShieldNormalized());
	}
}

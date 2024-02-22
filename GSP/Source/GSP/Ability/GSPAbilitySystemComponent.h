// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSPAbilitySystemComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GSPAbility, Log, All);

/**
 * 
 */
UCLASS()
class GSP_API UGSPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGSPAbilitySystemComponent();

protected:
	void BeginPlay() override;

	FGameplayTagContainer _OwnedGameplayTags;

};

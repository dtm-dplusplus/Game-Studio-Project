// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "GSPMovementAttributeSet.generated.h"


UCLASS()
class GSP_API UGSPMovementAttributeSet : public UGSPAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UGSPMovementAttributeSet, _MoveSpeed)

	ATTRIBUTE_ACCESSORS(UGSPMovementAttributeSet, _JumpHeight)
private:
	//~ Begin UAttributeSet interface
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//~ End UAttributeSet interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData _JumpHeight;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPMovementAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GSPCharacter.h"

void UGSPMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == Get_MoveSpeedAttribute())
	{
		if (const ACharacter* TargetCharacter = GetTypedOuter<ACharacter>())
		{
			if (UCharacterMovementComponent* MovementComponent = TargetCharacter->GetCharacterMovement())
			{
				MovementComponent->MaxWalkSpeed = NewValue;
			}
		}
	}

	if (Attribute == Get_JumpHeightAttribute())
	{
		if (const ACharacter* TargetCharacter = GetTypedOuter<ACharacter>())
		{
			if (UCharacterMovementComponent* MovementComponent = TargetCharacter->GetCharacterMovement())
			{
				MovementComponent->JumpZVelocity = NewValue;
			}
		}
	}
}

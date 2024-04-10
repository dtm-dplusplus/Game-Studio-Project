// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "GSPPlayerState.generated.h"


struct FOnAttributeChangeData;
class UGSPAttributeSet;

/**
 * Player State for GSP Characters
 */
UCLASS()
class GSP_API AGSPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGSPPlayerState();

};

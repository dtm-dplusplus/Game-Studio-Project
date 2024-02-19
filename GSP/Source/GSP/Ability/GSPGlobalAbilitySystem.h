// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GSPGlobalAbilitySystem.generated.h"

/**
 * 
 */
UCLASS()
class GSP_API UGSPGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void OnWorldBeginPlay(UWorld& InWorld) override;
};

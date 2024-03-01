// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GSPHealthComponent.generated.h"


class UGSPHealthSet;
class UGSPAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSP_API UGSPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGSPHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UGSPAbilitySystemComponent> _AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UGSPHealthSet> _HealthSet;
};
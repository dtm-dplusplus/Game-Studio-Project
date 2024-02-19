// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GSPAbilityComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GSPAbility, Log, All);

UCLASS( ClassGroup=(Ability), meta=(BlueprintSpawnableComponent) )
class GSP_API UGSPAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Abilities */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Ability", meta = (AllowPrivate))

public:	
	// Sets default values for this component's properties
	UGSPAbilityComponent();

	void UseAbility();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

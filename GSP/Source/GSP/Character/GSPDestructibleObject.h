// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "GSPDestructibleObject.generated.h"
class UNiagaraSystem;
class UNiagaraComponent;
class UGameplayEffect;
class UGSPAttributeSet;
struct FGSPAbilityInputAction;
struct FGameplayAbilitySpec;
class UGameplayAbility;
class UGSPAbilitySystemComponent;
UCLASS()
class GSP_API AGSPDestructibleObject : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AGSPDestructibleObject();

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End IAbilitySystemInterface */

	UGSPAttributeSet* GetGSPAttributeSet() const;

	virtual void Death();

	UFUNCTION(BlueprintImplementableEvent, Category = "GSP|GSPObject")
	void OnDeath();

	/** Call in BP once death actions are done */
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPObject")
	virtual void FinishDeath();


	UFUNCTION(BlueprintCallable, Category = "GSP|GSPObject|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPObject|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPObject|Attributes")
	float GetHealthRegenRate() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Ability Component */
	UPROPERTY()
	class UGSPAbilitySystemComponent* _AbilitySystemComponent;

	UPROPERTY()
	UGSPAttributeSet* _AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|GSPObject|Abilities")
	TSubclassOf<class UGameplayEffect> _DefaultAttributes;

	FDelegateHandle HealthChangedHandler;

	//virtual void HealthChanged(const FOnAttributeChangeData& Data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;




	// Particle System Played when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Character")
	TObjectPtr<UNiagaraSystem> _NiagaraSpawnFX;

	// Particle System that controls active Niagara FX when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Character")
	TObjectPtr<UNiagaraSystem> _NiagaraHitFX;

};

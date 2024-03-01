// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSPAttributeSet.h"
#include "GSPAbilitySystemComponent.generated.h"

class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(GSPAbility, Log, All);

USTRUCT()
struct FGSPAttributeInitializer
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGSPAttributeSet> AttributeSetType;

	// Data table reference to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere)
	UDataTable* InitializationData = nullptr;
};

/** Ability System Component for GSP
 * 
 */
UCLASS()
class GSP_API UGSPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FGSPAttributeInitializer> DefaultAttributes;

	//~ Begin UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End UAbilitySystemComponent interface

	//~ Begin UObject interface
	//virtual void BeginDestroy() override;
	//~ End UObject interface

	UFUNCTION(BlueprintCallable, Category = "AncientGame|Abilities")
	FGameplayAbilitySpecHandle GrantAbilityOfType(TSubclassOf<UGameplayAbility> AbilityType, bool bRemoveAfterActivation);

protected:
	void GrantDefaultAbilitiesAndAttributes();

	/** Handles generated from default abilities */
	TArray<FGameplayAbilitySpecHandle> DefaultAbilityHandles;

	/** Attributes added during gameplay*/
	UPROPERTY(transient)
	TArray<UAttributeSet*> AddedAttributes;
};

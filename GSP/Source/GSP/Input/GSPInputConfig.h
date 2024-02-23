// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GSPInputConfig.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GSPInput, Log, All)

class UInputAction;
class UObject;
struct FFrame;

/**
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FGSPInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 *	Non-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class GSP_API UGSPInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UGSPInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GSP|Character")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "GSP|Character")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// List of input actions used by the owner.
	// These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FGSPInputAction> NativeInputActions;

	// List of input actions used by the owner.
	// These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FGSPInputAction> AbilityInputActions;
};

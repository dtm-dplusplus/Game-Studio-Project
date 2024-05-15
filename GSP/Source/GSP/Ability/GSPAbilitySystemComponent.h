// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSPAttributeSet.h"
#include "GSPAbilitySystemComponent.generated.h"

struct FAbilityInputBinding;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(GSPAbility, Log, All);

/** Struct to hold input bindings for abilities */
USTRUCT(BlueprintType)
struct FAbilityInputBinding
{
	GENERATED_BODY()
	int32  InputID = 0;
	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
};

USTRUCT(BlueprintType)
struct FGSPAbilityInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	UInputAction* InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	TSubclassOf<UGameplayAbility> GameplayAbility;
};

/** Ability System Component for GSP
 * 
 */
UCLASS()
class GSP_API UGSPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	void SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	void ClearInputBinding(FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	void ClearAbilityBindings(UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	FGameplayAbilitySpecHandle GrantAbilityOfType(TSubclassOf<UGameplayAbility> AbilityType, bool bRemoveAfterActivation);

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ Begin UObject interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	//~ End UObject interface

	/** Handles generated from default abilities - These act as instances of the ability*/
	UPROPERTY(transient)
	TArray<FGameplayAbilitySpecHandle> DefaultAbilityHandles;

	/** Attributes added during gameplay*/
	UPROPERTY(transient)
	TArray<UAttributeSet*> AddedAttributes;

private:
	UPROPERTY(transient)
	TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;

	FAbilityInputBinding* FindAbilityInputBinding(UInputAction* InputAction);
	FGameplayAbilitySpec* FindAbilitySpec(FGameplayAbilitySpecHandle Handle);
	UEnhancedInputComponent* GetEnhancedInputComponent() const;
	void TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding);
	void OnAbilityInputPressed(UInputAction* InputAction);
	void OnAbilityInputReleased(UInputAction* InputAction);

	void ResetBindings();
	void RemoveEntry(UInputAction* InputAction);
};
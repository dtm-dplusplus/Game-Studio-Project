// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GSPAttributeSet.h"
#include "GSPAbilitySystemComponent.generated.h"

struct FAbilityInputBinding;
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
struct FAbilityInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Input")
	UInputAction* InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Input")
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
	UFUNCTION(BlueprintCallable, Category = Ability)
	void SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = Ability)
	void ClearInputBinding(FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(BlueprintCallable, Category = Ability)
	void ClearAbilityBindings(UInputAction* InputAction);

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FAbilityInputAction> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<FGSPAttributeInitializer> DefaultAttributes;

	//~ Begin UAbilitySystemComponent interface
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End UAbilitySystemComponent interface

	UFUNCTION(BlueprintCallable, Category = Ability)
	FGameplayAbilitySpecHandle GrantAbilityOfType(TSubclassOf<UGameplayAbility> AbilityType, bool bRemoveAfterActivation);

protected:
	void GrantDefaultAbilitiesAndAttributes();

	//~ Begin UObject interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	//~ End UObject interface

	/** Handles generated from default abilities */
	TArray<FGameplayAbilitySpecHandle> DefaultAbilityHandles;

	/** Attributes added during gameplay*/
	UPROPERTY(VisibleInstanceOnly, transient)
	TArray<UAttributeSet*> AddedAttributes;

private:
	UPROPERTY(transient)
	TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;

private:
	FAbilityInputBinding* FindAbilityInputBinding(UInputAction* InputAction);
	FGameplayAbilitySpec* FindAbilitySpec(FGameplayAbilitySpecHandle Handle);
	UEnhancedInputComponent* GetEnhancedInputComponent() const;
	void TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding);
	void OnAbilityInputPressed(UInputAction* InputAction);
	void OnAbilityInputReleased(UInputAction* InputAction);

	void ResetBindings();
	void RemoveEntry(UInputAction* InputAction);
};

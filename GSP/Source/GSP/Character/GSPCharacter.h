// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GSPCharacter.generated.h"

class UGSPAttributeSet;
struct FAbilityInputAction;
struct FGameplayAbilitySpec;
class UGameplayAbility;
class UGSPAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

// Declare a new log category
DECLARE_LOG_CATEGORY_EXTERN(GSPCharacter, Log, All);


/** Base class for all characters in the game - This should not be used directly, use subclasses of this type */
UCLASS()
class GSP_API AGSPCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AGSPCharacter(const FObjectInitializer& ObjectInitializer);

	bool IsAlive() const;

	virtual void Death();

	UFUNCTION(BlueprintImplementableEvent, Category = "GSP|GSCharacter")
	void OnDeath();

	/** Call in BP once death actions are done */
	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter")
	virtual void FinishDeath();

	virtual void InitializeAttributes();
	virtual void PossessedBy(AController* NewController) override;

protected:
	/* Ability Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	UGSPAbilitySystemComponent* _AbilitySystemComponent;

	// Reference to the _AttributeSet. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	UPROPERTY()
	UGSPAttributeSet* AttributeSetBase;

	FGameplayTag DeadTag;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GSP|Abilities")
	TSubclassOf<class UGameplayEffect> _DefaultAttributes;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> _MappingContext;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> _CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _FollowCamera;

	/** Projectile Spawn Point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> _ProjectileSpawnPoint;

	/**
	* Getters for attributes from GSPAttributeSet
	**/

	UFUNCTION(BlueprintCallable, Category = "GSP|Character|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetMaxShield() const;

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Attributes")
	float GetMoveSpeedBaseValue() const;

public:
	UFUNCTION(BlueprintCallable, Category = "GSP|Input")
	APlayerController* GetGSPPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Input")
	UEnhancedInputComponent* GetEnhancedInputComponent() const;

	/** IAbilitySystemInterface */
	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Ability")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSCharacter|Ability")
	UGSPAbilitySystemComponent* GetGSPAbilitySystemComponent() const;
	/** End of IAbilitySystemInterface */

	/** IGameplayTagAssetInterface */
	UFUNCTION(BlueprintCallable, Category = "GSP|GameplayTags")
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GameplayTags")
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GameplayTags")
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GameplayTags")
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	/** End of IGameplayTagAssetInterface */

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** APawn interface */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	/** End of APawn interface */

	/**
	* Setters for Attributes. Only use these in special cases, otherwise use a GE to change Attributes.
	*/

	virtual void SetHealth(float Health);
	virtual void SetStamina(float Stamina);
	virtual void SetShield(float Shield);
};
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayTagAssetInterface.h"
#include "GSPCharacter.generated.h"

struct FOnAttributeChangeData;
class AGSPPlayerState;
class UGSPAttributeSet;
struct FGSPAbilityInputAction;
struct FGameplayAbilitySpec;
class UGameplayAbility;
class UGSPAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

// Declare a new log category
DECLARE_LOG_CATEGORY_EXTERN(GSPCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGSPOnAttributeValueChanged, FGameplayAttribute, Attribute, float,
	NewValue, float, OldValue);


/** Base class for all characters in the game - This should not be used directly, use subclasses of this type */
UCLASS()
class GSP_API AGSPCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AGSPCharacter(const FObjectInitializer& ObjectInitializer);

	bool IsAlive() const;

	virtual void Death();

	UFUNCTION(BlueprintImplementableEvent, Category = "GSP|GSPCharacter")
	void OnDeath();

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter")
	void ResetState();

	/** Call in BP once death actions are done */
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter")
	virtual void FinishDeath();

	virtual void InitializeAbilities();

	/**
	* Getters for attributes from GSPAttributeSet
	**/
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Attributes")
	float GetHealth() const;

	// Returns health as a percentage of max health
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Attributes")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Attributes")
	float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Attributes")
	float GetMaxShield() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Attributes")
	float GetShieldNormalized();
protected:
	/* Ability Component */
	UPROPERTY(BlueprintReadWrite)
	UGSPAbilitySystemComponent* _AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	TObjectPtr<UGSPAttributeSet> _AttributeSet;

	FGameplayTag _DeadTag;


	// Attribute changed delegate handles
	FDelegateHandle HealthChangedDelegateHandle;


	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|GSPCharacter|Abilities")
	TSubclassOf<class UGameplayEffect> _DefaultAttributes;

	// Default Abilitity Input Action bindings. Granted on possession.
	UPROPERTY(EditAnywhere, Category = "GSP|GSPCharacter|Abilities")
	TArray<FGSPAbilityInputAction> _DefaultAbilities;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|GSPCharacter|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> _CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|GSPCharacter|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _FollowCamera;

	/** Projectile Spawn Point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|GSPCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> _ProjectileSpawnPoint;

public:
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Input")
	APlayerController* GetGSPPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter")
	AGSPPlayerState* GetGSPPlayerState() const;

	/** IAbilitySystemInterface */
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Ability")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Ability")
	UGSPAbilitySystemComponent* GetGSPAbilitySystemComponent() const;
	/** End of IAbilitySystemInterface */

	/** IGameplayTagAssetInterface */
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|GameplayTags")
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|GameplayTags")
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|GameplayTags")
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|GameplayTags")
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	/** End of IGameplayTagAssetInterface */

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
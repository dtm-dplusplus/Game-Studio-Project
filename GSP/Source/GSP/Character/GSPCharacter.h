// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilities/Public/GameplayAbilitySpecHandle.h"
#include "GameplayTags/Classes/GameplayTagAssetInterface.h"
#include "GSPCharacter.generated.h"

class UGSPCombatAttributeSet;
class UGSPCharacterAttributeSet;
class UGSPMovementAttributeSet;
struct FAbilityInputAction;
struct FGameplayAbilitySpec;
class UGameplayAbility;
class UGSPAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

// Declare a new log category
DECLARE_LOG_CATEGORY_EXTERN(GSPCharacter, Log, All);


/** Base class for all characters in the game */
UCLASS(config = Game)
class GSP_API AGSPCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AGSPCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GSP|Character")
	APlayerController* GetGSPPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|Character")
	UEnhancedInputComponent* GetEnhancedInputComponent() const;

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End of IAbilitySystemInterface */

	class UGSPHealthComponent* GetHealthComponent() const;
public:
	/** UGSPMovementAttributeSet */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	const UGSPMovementAttributeSet* GetMovementAttributeSet() const;
	/** End of UGSPMovementAttributeSet */

	/** UGSPCharacterAttributeSet */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	const UGSPCharacterAttributeSet* GetCharacterAttributeSet() const;
	/** End of UGSPCharacterAttributeSet */

	/** UGSPCombatAttributeSet */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	const UGSPCombatAttributeSet* GetCombatAttributeSet() const;
	/** End of UGSPCombatAttributeSet */

public:
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

protected:
	/* Ability Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	UGSPAbilitySystemComponent* _AbilitySystemComponent;

	/* Health Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Character")
	UGSPHealthComponent* _HealthComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> _MappingContext;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> _CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _FollowCamera;

	/** Projectile Spawn Point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> _ProjectileSpawnPoint;
};

#pragma once
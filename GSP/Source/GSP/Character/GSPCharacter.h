// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTags/Classes/GameplayTagAssetInterface.h"
#include "GSPCharacter.generated.h"

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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GSP|Character")
	APlayerController* GetGSPPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "GSP|Character")
	APlayerState* GetGSPPlayerState() const;

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
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
	/* Ability Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	class UGSPAbilitySystemComponent* _AbilitySystemComponent;

	/** Grants an ability */
	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode);

	/** ACtivates an ability */
	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	void ActivateAbility(int32 InputCode);

	/** Cancels an ability */
	UFUNCTION(BlueprintCallable, Category = "GSP|Ability")
	void CancelAbility(const FGameplayTagContainer& CancelWithTags);

	/** Attribute Set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	const class UGSPAttributeSet* _AttributeSet;

	/** Character Health */
	UFUNCTION(BlueprintPure, Category = "GSP|Ability|Attribute")
	float GetCharacterHealth() const;

	UFUNCTION(BlueprintPure, Category = "GSP|Ability|Attribute")
	float GetCharacterMaxHealth() const;
private:	
    

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UInputMappingContext> _MappingContext;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> _JumpAction;

    /** Move Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> _MoveAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> _LookAction;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> _CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> _FollowCamera;

	/** Projectile Spawn Point */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GSP", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> _ProjectileSpawnPoint;

protected:

	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);

	/** Called for looking input */
	virtual void Look(const FInputActionValue& Value);

	/** Called for jumping input */
	void Jump() override;
	
	/** Called to Stop Jumping*/
	void StopJumping() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;



private:


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return _CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return _FollowCamera; }
};

#pragma once
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GSPCharacter.generated.h"

class UInputAction;

// Declare a new log category
DECLARE_LOG_CATEGORY_EXTERN(GSPCharacter, Log, All);

UCLASS(config = Game)
class GSP_API AGSPCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Use Ability Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAbilityAction;

	/* Ability Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGSPAbilitySystemComponent> _AbilitySystemComponent;

public:
	AGSPCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FGameplayTag _DeadTag;
protected:

	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);

	/** Called for looking input */
	virtual void Look(const FInputActionValue& Value);

	/** Called for jumping input */
	void Jump() override;
	
	/** Called to Stop Jumping*/
	void StopJumping() override;

	/** Called for using elemental ability input */
	void UseAbility();

protected:
	// APawn interface
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void BeginPlay() override;
};

#pragma once
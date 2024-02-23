// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "../Ability/GSPAttributeSet.h"


DEFINE_LOG_CATEGORY(GSPCharacter)


AGSPCharacter::AGSPCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	_CameraBoom->SetupAttachment(RootComponent);
	_CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	_CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	_FollowCamera->SetupAttachment(_CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	_FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a projectile spawn point
	_ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	_ProjectileSpawnPoint->SetupAttachment(RootComponent);

	// Set up Abilities
	_AbilitySystemComponent = CreateDefaultSubobject<UGSPAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void AGSPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_MappingContext, 0);
		}
	}

	// Initialise Attribute Set
	if(IsValid(_AbilitySystemComponent))
	{
		_AttributeSet = _AbilitySystemComponent->GetSet<UGSPAttributeSet>();
	}
}

APlayerController* AGSPCharacter::GetGSPPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

APlayerState* AGSPCharacter::GetGSPPlayerState() const
{
	return GetGSPPlayerController()->PlayerState;
}

UAbilitySystemComponent* AGSPCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(_AbilitySystemComponent);
}

void AGSPCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (_AbilitySystemComponent)
	{
		_AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	}
}

bool AGSPCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (_AbilitySystemComponent)
	{
		return _AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool AGSPCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (_AbilitySystemComponent)
	{
		return _AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool AGSPCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (_AbilitySystemComponent)
	{
		return _AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

void AGSPCharacter::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode)
{
	// Check if we have a valid AbilitySystemComponent and AbilityClass - Net code may not be necessary
	if(GetLocalRole() == ROLE_Authority && IsValid(_AbilitySystemComponent) &&  IsValid(AbilityClass))
	{
		UGameplayAbility* Ability = AbilityClass->GetDefaultObject<UGameplayAbility>();

		if(IsValid(Ability))
		{
			// Create a new FGameplayAbilitySpec and give it to the character
			FGameplayAbilitySpec AbilitySpec(Ability, Level, InputCode, this);

			// Give the ability to the character
			_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, Level, InputCode, this));
		}
	}
}

void AGSPCharacter::ActivateAbility(int32 InputCode)
{
	if (IsValid(_AbilitySystemComponent))
	{
		// Can this be bound to an input action?
		_AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
	}
}

void AGSPCharacter::CancelAbility(const FGameplayTagContainer& CancelWithTags)
{
	if(IsValid(_AbilitySystemComponent))
	{
		_AbilitySystemComponent->CancelAbilities(&CancelWithTags);
	}
}

float AGSPCharacter::GetCharacterHealth() const
{
	if (_AttributeSet)
	{
		return _AttributeSet->Get_Health();
	}

	return 0.0f;
}

float AGSPCharacter::GetCharacterMaxHealth() const
{
	if (_AttributeSet)
	{
		return _AttributeSet->Get_MaxHealth();
	}

	return 0.0f;
}

void AGSPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(_JumpAction, ETriggerEvent::Triggered, this, &AGSPCharacter::Jump);
		EnhancedInputComponent->BindAction(_JumpAction, ETriggerEvent::Completed, this, &AGSPCharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(_MoveAction, ETriggerEvent::Triggered, this, &AGSPCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(_LookAction, ETriggerEvent::Triggered, this, &AGSPCharacter::Look);
	}
}


void AGSPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// UE_LOG(Character, Log, TEXT("Move Input X: %f Y: %f"), MovementVector.X, MovementVector.Y);
	}
}

void AGSPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		// UE_LOG(GSPCharacter, Log, TEXT("Look Input X: %f Y: %f"), LookAxisVector.X, LookAxisVector.Y);
	}
}

void AGSPCharacter::Jump()
{
	Super::Jump();

	UE_LOG(GSPCharacter, Log, TEXT("Jump"));
}

void AGSPCharacter::StopJumping()
{
	Super::StopJumping();
}

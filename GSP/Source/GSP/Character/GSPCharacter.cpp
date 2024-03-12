// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GSPCharacterAttributeSet.h"
#include "GSPCombatAttributeSet.h"
#include "GSPMovementAttributeSet.h"
#include "Components/ArrowComponent.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/GSPGlobalAbilitySystem.h"
#include "GSPHealthComponent.h"

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

	// Set up Ability component
	_AbilitySystemComponent = CreateDefaultSubobject<UGSPAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Set up Health component
	_HealthComponent = CreateDefaultSubobject<UGSPHealthComponent>(TEXT("HealthComponent"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void AGSPCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_MappingContext, 0);
		}
	}

	// Set up Attributes
	check(_AbilitySystemComponent)
	{
		if(const UGSPMovementAttributeSet* MovementAttribSet
			= Cast<UGSPMovementAttributeSet>(_AbilitySystemComponent->GetAttributeSet(UGSPMovementAttributeSet::StaticClass())))
		{
			GetCharacterMovement()->JumpZVelocity = MovementAttribSet->Get_JumpHeight();
			GetCharacterMovement()->MaxWalkSpeed = MovementAttribSet->Get_MoveSpeed();
		}
	}

	// Set up Health
	_HealthComponent->Initialize(_AbilitySystemComponent);
}

void AGSPCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGSPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

APlayerController* AGSPCharacter::GetGSPPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

/////////////////////// Components //////////////////////
UEnhancedInputComponent* AGSPCharacter::GetEnhancedInputComponent() const
{
	return Cast<UEnhancedInputComponent>(GetGSPPlayerController()->InputComponent);
}

UAbilitySystemComponent* AGSPCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(_AbilitySystemComponent);
}

UGSPAbilitySystemComponent* AGSPCharacter::GetGSPAbilitySystemComponent() const
{
	return Cast<UGSPAbilitySystemComponent>(_AbilitySystemComponent);
}

UGSPHealthComponent* AGSPCharacter::GetHealthComponent() const
{
	return Cast<UGSPHealthComponent>(_HealthComponent); 
}


/////////////////////// Attribute Sets //////////////////////
const UGSPMovementAttributeSet* AGSPCharacter::GetMovementAttributeSet() const
{
	if(_AbilitySystemComponent)
	{
		if(const UGSPMovementAttributeSet* MovementAttribSet
						= Cast<UGSPMovementAttributeSet>(_AbilitySystemComponent->GetAttributeSet(UGSPMovementAttributeSet::StaticClass())))
		{
			return MovementAttribSet;
		}
		return nullptr;
	}
	return nullptr;
}

const UGSPCharacterAttributeSet* AGSPCharacter::GetCharacterAttributeSet() const
{
	if (_AbilitySystemComponent)
	{
		if (const UGSPCharacterAttributeSet* CharAttribSet
			= Cast<UGSPCharacterAttributeSet>(_AbilitySystemComponent->GetAttributeSet(UGSPCharacterAttributeSet::StaticClass())))
		{
			return CharAttribSet;
		}
		return nullptr;
	}
	return nullptr;
}

const UGSPCombatAttributeSet* AGSPCharacter::GetCombatAttributeSet() const
{
	if(_AbilitySystemComponent)
	{
		if (const UGSPCombatAttributeSet* CombatAttribSet
				= Cast<UGSPCombatAttributeSet>(_AbilitySystemComponent->GetAttributeSet(UGSPCombatAttributeSet::StaticClass())))
		{
			return CombatAttribSet;
		}
		return nullptr;
	}
	return nullptr;
}


////////////////////// Gameplay Tags //////////////////////
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
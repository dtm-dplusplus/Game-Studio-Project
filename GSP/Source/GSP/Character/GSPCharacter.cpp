
#include "GSPCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GSPPlayerState.h"
#include "Components/ArrowComponent.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "../Ability/GSPAttributeSet.h"

DEFINE_LOG_CATEGORY(GSPCharacter)

AGSPCharacter::AGSPCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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
	//_AbilitySystemComponent = CreateDefaultSubobject<UGSPAbilitySystemComponent>(TEXT("_AbilitySystemComponent"));


	// _AbilitySystemComponent needs to be updated at a high frequency
	NetUpdateFrequency = 100.0f;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = false;

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag("Actor.State.Dead");
}

bool AGSPCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AGSPCharacter::Death()
{
	OnDeath();
}

void AGSPCharacter::FinishDeath()
{
	Destroy();
}

void AGSPCharacter::InitializeAbilities()
{
	// Reset/Remove abilities if we had already added them
	//{
	//	for (UAttributeSet* AttribSetInstance : AddedAttributes)
	//	{
	//		RemoveSpawnedAttribute(AttribSetInstance);
	//	}

	//	for (FGameplayAbilitySpecHandle AbilityHandle : DefaultAbilityHandles)
	//	{
	//		SetRemoveAbilityOnEnd(AbilityHandle);
	//	}

	//	AddedAttributes.Empty(DefaultAttributes.Num());
	//	DefaultAbilityHandles.Empty(DefaultAbilities.Num());
	//}

	// Default abilities
	if(_AbilitySystemComponent)
	{
		for (auto& [InputAction, Ability] : DefaultAbilities)
		{
			// Give Ability
			if (*Ability)
			{
				FGameplayAbilitySpecHandle AbilitySpec = _AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));

				// If an InputAction was specified, bind it to the ability
				if (InputAction)
				{
					_AbilitySystemComponent->SetInputBinding(InputAction, AbilitySpec);
				}

				// Add to DefaultAbilityHandles
				_AbilitySystemComponent->DefaultAbilityHandles.Add(AbilitySpec);
			}
		}
	}
}


void AGSPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AGSPPlayerState* PS = GetPlayerState<AGSPPlayerState>())
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		_AbilitySystemComponent = Cast<UGSPAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the _AttributeSet for convenience attribute functions
		AttributeSetBase = PS->GetGSPAttributeSet();

		// Apply our defualt gameplay effect to create our attributes
		if (_AbilitySystemComponent && _DefaultAttributes)
		{
			FGameplayEffectContextHandle EffectContext = _AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = _AbilitySystemComponent->MakeOutgoingSpec(_DefaultAttributes, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = _AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
			}
		}
		else
		{
			UE_LOG(GSPCharacter, Error, TEXT("Missing _DefaultAttributes or _AbilitySystemComponent for %s."), *GetName());
		}

		// Initalize our abilities and input bindings
		InitializeAbilities();

		//AddStartupEffects();

		// If we are dead, respawn with max attributes
		if (_AbilitySystemComponent->GetTagCount(DeadTag) > 0)
		{
			// Set to max for respawn
			SetHealth(GetMaxHealth());
			SetStamina(GetMaxStamina());
			SetShield(GetMaxShield());
		}

		// Remove Dead tag
		_AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(DeadTag));

		UE_LOG(GSPCharacter, Warning, TEXT("AGSPCharacter::PossessedBy %s"), *GetName());
	}
}

int32 AGSPCharacter::GetCharacterLevel() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetCharacterLevel();
	}

	return 0;
}

float AGSPCharacter::GetHealth() const
{
	if(AttributeSetBase)
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float AGSPCharacter::GetHealthNormalized() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetHealth() / AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float AGSPCharacter::GetMoveSpeed() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.0f;
}

float AGSPCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMoveSpeedAttribute().
			GetGameplayAttributeData(AttributeSetBase)->GetBaseValue();
	}

	return 0.0f;
}

float AGSPCharacter::GetMaxHealth() const
{
	if(AttributeSetBase)
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float AGSPCharacter::GetStamina() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float AGSPCharacter::GetMaxStamina() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMaxStamina();
	}

	return 0.0f;
}

float AGSPCharacter::GetShield() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetShield();
	}

	return 0.0f;
}

float AGSPCharacter::GetMaxShield() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMaxShield();
	}

	return 0.0f;
}

void AGSPCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_MappingContext, 0);
		}
	}

	if(!_AbilitySystemComponent)
	{
		UE_LOG(GSPCharacter, Error, TEXT("%s() Missing _AbilitySystemComponent for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	if (!_DefaultAttributes)
	{
		UE_LOG(GSPCharacter, Error, TEXT("%s() Missing _DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}
}

void AGSPCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGSPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGSPCharacter::SetHealth(float Health)
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetHealth(Health);
	}

}

void AGSPCharacter::SetStamina(float Stamina)
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetStamina(Stamina);
	}
}

void AGSPCharacter::SetShield(float Shield)
{
	if(AttributeSetBase)
	{
		AttributeSetBase->SetShield(Shield);
	}
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
		return _AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}
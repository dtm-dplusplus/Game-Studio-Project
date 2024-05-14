
#include "GSPCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
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
	// Create ability system component, and set it to be explicitly replicated
	_AbilitySystemComponent = CreateDefaultSubobject<UGSPAbilitySystemComponent>(TEXT("_AbilitySystemComponent")); //Component
	_AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself
	_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// automatically registers the _AttributeSet with the _AbilitySystemComponent
	_AttributeSet = CreateDefaultSubobject<UGSPAttributeSet>(TEXT("_AttributeSet"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	NetUpdateFrequency = 100.0f;

	// _AbilitySystemComponent needs to be updated at a high frequency
	NetUpdateFrequency = 100.0f;
	bAlwaysRelevant = true;

	PrimaryActorTick.bCanEverTick = true;
}

void AGSPCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	// Check for and handle death
	if (!IsAlive())
	{
		Death();
		UE_LOG(LogTemp, Warning, TEXT("AGSPPlayerState::HealthChanged: %s is dead."), *GetName());
	}

	UE_LOG(LogTemp, Warning, TEXT("HealthChanged: %f"), Data.NewValue);
}


bool AGSPCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AGSPCharacter::Death()
{
	OnDeath();
}

void AGSPCharacter::ResetState()
{
	// Reset health
	//_AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Game.Effect.Cooldown"));
	_AttributeSet->SetHealth(_AttributeSet->GetMaxHealth());
	_AttributeSet->HealthChangeDelegate.Broadcast(GetHealth(), GetHealthNormalized());
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
	//	DefaultAbilityHandles.Empty(_DefaultAbilities.Num());
	//}

	// Default abilities
	if(_AbilitySystemComponent)
	{
		for (auto& [InputAction, Ability] : _DefaultAbilities)
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


float AGSPCharacter::GetHealth() const
{
	if(_AttributeSet)
	{
		return _AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AGSPCharacter::GetHealthNormalized() const
{
	if (_AttributeSet)
	{
		return _AttributeSet->GetHealth() / _AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AGSPCharacter::GetMaxHealth() const
{
	if(_AttributeSet)
	{
		return _AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AGSPCharacter::GetShield() const
{
	if (_AttributeSet)
	{
		return _AttributeSet->GetShield();
	}

	return 0.0f;
}

float AGSPCharacter::GetMaxShield() const
{
	if (_AttributeSet)
	{
		return _AttributeSet->GetMaxShield();
	}

	return 0.0f;
}

float AGSPCharacter::GetShieldNormalized()
{
	if (_AttributeSet)
	{
		return _AttributeSet->GetShieldNormalized();
	}

	return 0.0f;
}

void AGSPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	// Apply our defualt gameplay effect to create our attributes
	if (_AbilitySystemComponent && _DefaultAttributes)
	{
		FGameplayEffectContextHandle EffectContext = _AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		if (const FGameplayEffectSpecHandle NewHandle = _AbilitySystemComponent->MakeOutgoingSpec(_DefaultAttributes, 0.0f, EffectContext); NewHandle.IsValid())
		{
			_AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
	else
	{
		UE_LOG(GSPCharacter, Error, TEXT("Missing _DefaultAttributes or _AbilitySystemComponent for %s."), *GetName());
	}

	// Initalize our abilities and input bindings
	InitializeAbilities();

	//AddStartupEffects();

	if (_AbilitySystemComponent)
	{
		// Health change callbacks
		HealthChangedDelegateHandle = _AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(_AttributeSet->GetHealthAttribute()).AddUObject(this, &AGSPCharacter::HealthChanged);
	}

	if (!_AbilitySystemComponent)
	{
		UE_LOG(GSPCharacter, Error, TEXT("%s() Missing _AbilitySystemComponent for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	if (!_DefaultAttributes)
	{
		UE_LOG(GSPCharacter, Error, TEXT("%s() Missing _DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
	}

	_AttributeSet->SetHealth(_AttributeSet->GetMaxHealth());
}

void AGSPCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

APlayerController* AGSPCharacter::GetGSPPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

AGSPPlayerState* AGSPCharacter::GetGSPPlayerState() const
{
	return Cast<AGSPPlayerState>(GetPlayerState());
}

/////////////////////// Components //////////////////////
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
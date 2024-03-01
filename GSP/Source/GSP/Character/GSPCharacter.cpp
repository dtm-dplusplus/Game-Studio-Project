// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "../Ability/GSPAbilitySystemComponent.h"
#include "../Ability/GSPAttributeSet.h"
#include "../Ability/GSPGlobalAbilitySystem.h"


DEFINE_LOG_CATEGORY(GSPCharacter)

namespace AbilityInputBindingComponent_Impl
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}

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

	if (UGSPGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGSPGlobalAbilitySystem>(GetWorld()))
	{
		GlobalAbilitySystem->AddASC(_AbilitySystemComponent);
	}

	RunAbilitySystemSetup();
}

APlayerController* AGSPCharacter::GetGSPPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

UEnhancedInputComponent* AGSPCharacter::GetEnhancedInputComponent() const
{
	return Cast<UEnhancedInputComponent>(GetGSPPlayerController()->InputComponent);
}

FVector2D AGSPCharacter::GetInputActionValue2D(UInputAction* InputAction)
{
	if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
	{
		return EIC->BindActionValue(InputAction).GetValue().Get<FVector2D>();
	}

	return FVector2D();
}

UAbilitySystemComponent* AGSPCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(_AbilitySystemComponent);
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

//void AGSPCharacter::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode)
//{
//	// Check if we have a valid AbilitySystemComponent and AbilityClass - Net code may not be necessary
//	if(GetLocalRole() == ROLE_Authority && IsValid(_AbilitySystemComponent) &&  IsValid(AbilityClass))
//	{
//		UGameplayAbility* Ability = AbilityClass->GetDefaultObject<UGameplayAbility>();
//
//		if(IsValid(Ability))
//		{
//			// Create a new FGameplayAbilitySpec and give it to the character
//			FGameplayAbilitySpec AbilitySpec(Ability, Level, InputCode, this);
//
//			// Give the ability to the character
//			_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, Level, InputCode, this));
//		}
//	}
//}
//
//void AGSPCharacter::ActivateAbility(int32 InputCode)
//{
//	if (_AbilitySystemComponent)
//	{
//		// Can this be bound to an input action?
//		_AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
//	}
//}
//
//void AGSPCharacter::CancelAbility(const FGameplayTagContainer& CancelWithTags)
//{
//	if(IsValid(_AbilitySystemComponent))
//	{
//		_AbilitySystemComponent->CancelAbilities(&CancelWithTags);
//	}
//}

void AGSPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*ResetBindings();

	RunAbilitySystemSetup();*/

	// Set up action bindings
	//if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent()) {

		//Jumping
		//EnhancedInputComponent->BindAction(_JumpAction, ETriggerEvent::Triggered, this, &AGSPCharacter::Jump);
		//EnhancedInputComponent->BindAction(_JumpAction, ETriggerEvent::Completed, this, &AGSPCharacter::StopJumping);

		////Moving
		//EnhancedInputComponent->BindAction(_MoveAction, ETriggerEvent::Triggered, this, &AGSPCharacter::IA_Move);

		////Looking
		//EnhancedInputComponent->BindAction(_LookAction, ETriggerEvent::Triggered, this, &AGSPCharacter::IA_Look);

		//// Abilities
		//EnhancedInputComponent->BindAction(_AttackAction, ETriggerEvent::Triggered, this, &AGSPCharacter::IA_Attack);
		//EnhancedInputComponent->BindAction(_DefendAction, ETriggerEvent::Triggered, this, &AGSPCharacter::IA_Defend);
		//EnhancedInputComponent->BindAction(_SupportAction, ETriggerEvent::Triggered, this, &AGSPCharacter::IA_Support);
		//SetInputBinding(_JumpAction, _AbilitySystemComponent->GrantAbilityOfType(_JumpAbility, false));
		//
		//EIComp->BindAction(_JumpAction, ETriggerEvent::Started, this, &AGSPCharacter::OnAbilityInputPressed, _JumpAction);
	//}
}


/** Ability System */
void AGSPCharacter::RunAbilitySystemSetup()
{
	ResetBindings();

	//if(UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
	//{
	//	// Bind Default Abilities (Default player controls)
	//	for (const auto& Ability : MappedAbilities)
	//	{
	//		UInputAction* InputAction = Ability.Key;

	//		// Pressed event (This overload doesn't work)
	//		EIC->BindAction(InputAction, ETriggerEvent::Started, this, &AGSPCharacter::OnAbilityInputPressed, InputAction);

	//		// Released event
	//		EIC->BindAction(InputAction, ETriggerEvent::Completed, this, &AGSPCharacter::OnAbilityInputReleased, InputAction);
	//	}
	//}

	//// Assign Input IDs
	//for (auto& InputBinding : MappedAbilities)
	//{
	//	const int32 NewInputID = AbilityInputBindingComponent_Impl::GetNextInputID();
	//	InputBinding.Value.InputID = NewInputID;

	//	for (const FGameplayAbilitySpecHandle AbilityHandle : InputBinding.Value.BoundAbilitiesStack)
	//	{
	//		if (FGameplayAbilitySpec* FoundAbility = FindAbilitySpec(AbilityHandle))
	//		{
	//			FoundAbility->InputID = NewInputID;
	//		}
	//	}
	//}

	for(auto& [InputAction, GameplayAbility] : _AbilityInputActions)
	{
		SetInputBinding(InputAction, _AbilitySystemComponent->GrantAbilityOfType(GameplayAbility, false));

		//MappedAbilities.Add(InputAction);
		UE_LOG(GSPAbility, Log, TEXT("Mapped Ability: %s to ID %d"), *InputAction->GetName(), FindAbilityInputBinding(InputAction)->InputID);
	}
}



FGameplayAbilitySpec* AGSPCharacter::FindAbilitySpec(FGameplayAbilitySpecHandle Handle)
{
	if (!_AbilitySystemComponent)
	{
		return nullptr;
	}

	return _AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
}

void AGSPCharacter::RemoveEntry(UInputAction* InputAction)
{
	if (FAbilityInputBinding* Bindings = MappedAbilities.Find(InputAction))
	{
		if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
		{
			EIC->RemoveBindingByHandle(Bindings->OnPressedHandle);
			EIC->RemoveBindingByHandle(Bindings->OnReleasedHandle);
		}

		for (const FGameplayAbilitySpecHandle AbilityHandle : Bindings->BoundAbilitiesStack)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpec(AbilityHandle);
			if (AbilitySpec && AbilitySpec->InputID == Bindings->InputID)
			{
				AbilitySpec->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
			}
		}

		MappedAbilities.Remove(InputAction);
	}
}

////////////////////////////////  Input Binding //////////////////////////////////////
void AGSPCharacter::SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle)
{
	// Nullptr if no input is bound, will return and override if found
	FAbilityInputBinding* AbilityInputBinding = FindAbilityInputBinding(InputAction);

	if (AbilityInputBinding)
	{
		// Remove old input binding - Mark as invalid
		FGameplayAbilitySpec* OldBoundAbility = FindAbilitySpec(AbilityInputBinding->BoundAbilitiesStack.Top());
		if (OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID)
		{
			OldBoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
		}
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
		AbilityInputBinding->InputID = AbilityInputBindingComponent_Impl::GetNextInputID();
	}

	// Get Ability Spec from Ability Handle
	if (FGameplayAbilitySpec* BindingAbility = FindAbilitySpec(AbilityHandle))
	{
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	AbilityInputBinding->BoundAbilitiesStack.Push(AbilityHandle);
	TryBindAbilityInput(InputAction, *AbilityInputBinding);
}

void AGSPCharacter::ClearInputBinding(FGameplayAbilitySpecHandle AbilityHandle)
{
	if (FGameplayAbilitySpec* FoundAbility = FindAbilitySpec(AbilityHandle))
	{
		// Find the mapping for this ability
		auto MappedIterator = MappedAbilities.CreateIterator();
		while (MappedIterator)
		{
			if (MappedIterator.Value().InputID == FoundAbility->InputID)
			{
				break;
			}

			++MappedIterator;
		}

		if (MappedIterator)
		{
			FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

			if (AbilityInputBinding.BoundAbilitiesStack.Remove(AbilityHandle) > 0)
			{
				if (AbilityInputBinding.BoundAbilitiesStack.Num() > 0)
				{
					FGameplayAbilitySpec* StackedAbility = FindAbilitySpec(AbilityInputBinding.BoundAbilitiesStack.Top());
					if (StackedAbility && StackedAbility->InputID == 0)
					{
						StackedAbility->InputID = AbilityInputBinding.InputID;
					}
				}
				else
				{
					// NOTE: This will invalidate the `AbilityInputBinding` ref above
					RemoveEntry(MappedIterator.Key());
				}
				// DO NOT act on `AbilityInputBinding` after here (it could have been removed)


				FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
			}
		}
	}
}

void AGSPCharacter::ClearAbilityBindings(UInputAction* InputAction)
{
	RemoveEntry(InputAction);
}

FAbilityInputBinding* AGSPCharacter::FindAbilityInputBinding(UInputAction* InputAction)
{
	return MappedAbilities.Find(InputAction);
}


void AGSPCharacter::ResetBindings()
{
	for (auto& InputBinding : MappedAbilities)
	{
		if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
		{
			EIC->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			EIC->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}

		if (_AbilitySystemComponent)
		{
			const int32 ExpectedInputID = InputBinding.Value.InputID;

			for (FGameplayAbilitySpecHandle AbilityHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				FGameplayAbilitySpec* FoundAbility = FindAbilitySpec(AbilityHandle);
				if (FoundAbility && FoundAbility->InputID == ExpectedInputID)
				{
					FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
				}
			}
		}
	}
	// AbilityComponent = nullptr;
}

void AGSPCharacter::TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding)
{
	if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
	{
		// Pressed event
		if (AbilityInputBinding.OnPressedHandle == 0)
		{
			AbilityInputBinding.OnPressedHandle = EIC->BindAction(InputAction, ETriggerEvent::Started, this, &AGSPCharacter::OnAbilityInputPressed, InputAction).GetHandle();
		}
		else
		{
			UE_LOG(GSPAbility, Warning, TEXT("TryBindAbilityInput: AbilityInputBinding.OnPressedHandle is already set!"));
		}

		// Released event
		if (AbilityInputBinding.OnReleasedHandle == 0)
		{
			AbilityInputBinding.OnReleasedHandle = EIC->BindAction(InputAction, ETriggerEvent::Completed, this, &AGSPCharacter::OnAbilityInputReleased, InputAction).GetHandle();
		}
		else
		{
			UE_LOG(GSPAbility, Warning, TEXT("TryBindAbilityInput: AbilityInputBinding.OnReleasedHandle is already set!"));
		}
	}
	else
	{
		UE_LOG(GSPAbility, Warning, TEXT("TryBindAbilityInput: EnhancedInputComponent is not valid!"));
	}
}


void AGSPCharacter::OnAbilityInputPressed(UInputAction* InputAction)
{
	// The AbilitySystemComponent may not have been valid when we first bound input... try again.
	/*if (!IsActive())
	{
		RunAbilitySystemSetup();
	}*/
	if (_AbilitySystemComponent)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
		{
			UE_LOG(GSPAbility, Warning, TEXT("OnAbilityInputPressed: IA: %s ID: %d"), *InputAction->GetName(), FoundBinding->InputID);
			_AbilitySystemComponent->AbilityLocalInputPressed(FoundBinding->InputID);
		}
	}
}

void AGSPCharacter::OnAbilityInputReleased(UInputAction* InputAction)
{
	if (_AbilitySystemComponent)
	{
		const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
		{
			UE_LOG(GSPAbility, Warning, TEXT("OnAbilityInputReleased: IA: %s ID: %d"), *InputAction->GetName(), FoundBinding->InputID);
			_AbilitySystemComponent->AbilityLocalInputReleased(FoundBinding->InputID);
		}
	}
}
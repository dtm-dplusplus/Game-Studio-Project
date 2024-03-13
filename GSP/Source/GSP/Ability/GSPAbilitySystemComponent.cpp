// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPAbilitySystemComponent.h"

#include "GSPGlobalAbilitySystem.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY(GSPAbility)

namespace AbilityInputBindingComponent_Impl
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}




void UGSPAbilitySystemComponent::GrantDefaultAbilitiesAndAttributes()
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
	{
		DefaultAbilityHandles.Reserve(DefaultAbilities.Num());
		for (auto& [InputAction, Ability] : DefaultAbilities)
		{
			// Give Ability
			if (*Ability)
			{
				DefaultAbilityHandles.Add(GiveAbility(FGameplayAbilitySpec(Ability)));
			}

			// Bind Input to Ability
			if (InputAction)
			{
				SetInputBinding(InputAction, DefaultAbilityHandles.Last());
			}
		}
	}

	// Default attributes
	/*{
		for (const FGSPAttributeInitializer& Attributes : DefaultAttributes)
		{
			if (Attributes.AttributeSetType)
			{
				UGSPAttributeSet* NewAttribSet = NewObject<UGSPAttributeSet>(this, Attributes.AttributeSetType);
				if (Attributes.InitializationData)
				{
					NewAttribSet->InitFromMetaDataTable(Attributes.InitializationData);
				}
				AddedAttributes.Add(NewAttribSet);
				AddAttributeSetSubobject(NewAttribSet);
			}
		}
	}*/
}

void UGSPAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	

	GrantDefaultAbilitiesAndAttributes();
}

void UGSPAbilitySystemComponent::BeginDestroy()
{
	if (UGSPGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGSPGlobalAbilitySystem>(GetWorld()))
	{
		GlobalAbilitySystem->RemoveASC(this);
	}

	Super::BeginDestroy();
}

void UGSPAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (UGSPGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGSPGlobalAbilitySystem>(GetWorld()))
	{
		GlobalAbilitySystem->AddASC(this);
		UE_LOG(GSPAbility, Warning, TEXT("InitAbilityActorInfo: %s"), *GetName());
	}
}

FGameplayAbilitySpecHandle UGSPAbilitySystemComponent::GrantAbilityOfType(TSubclassOf<UGameplayAbility> AbilityType,
                                                                          bool bRemoveAfterActivation)
{
	FGameplayAbilitySpecHandle AbilityHandle;
	if (AbilityType)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityType);
		AbilitySpec.RemoveAfterActivation = bRemoveAfterActivation;

		AbilityHandle = GiveAbility(AbilitySpec);
	}
	return AbilityHandle;
}


FGameplayAbilitySpec* UGSPAbilitySystemComponent::FindAbilitySpec(FGameplayAbilitySpecHandle Handle)
{
	return FindAbilitySpecFromHandle(Handle);
}

UEnhancedInputComponent* UGSPAbilitySystemComponent::GetEnhancedInputComponent() const
{
	if (AActor* Owner = GetOwner())
	{
		if (const APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
		{
				return Cast<UEnhancedInputComponent>(PC->InputComponent);
		}
	}
	return nullptr;	
}

void UGSPAbilitySystemComponent::RemoveEntry(UInputAction* InputAction)
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

FAbilityInputBinding* UGSPAbilitySystemComponent::FindAbilityInputBinding(UInputAction* InputAction)
{
	return MappedAbilities.Find(InputAction);
}


void UGSPAbilitySystemComponent::ResetBindings()
{
	for (auto& InputBinding : MappedAbilities)
	{
		if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
		{
			EIC->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			EIC->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}

		
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
	// AbilityComponent = nullptr;
}

////////////////////////////////  Input Binding //////////////////////////////////////
void UGSPAbilitySystemComponent::SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle)
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

void UGSPAbilitySystemComponent::ClearInputBinding(FGameplayAbilitySpecHandle AbilityHandle)
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

void UGSPAbilitySystemComponent::ClearAbilityBindings(UInputAction* InputAction)
{
	RemoveEntry(InputAction);
}
void UGSPAbilitySystemComponent::TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding)
{
	if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
	{
		// Pressed event
		if (AbilityInputBinding.OnPressedHandle == 0)
		{
			AbilityInputBinding.OnPressedHandle = EIC->BindAction(InputAction, ETriggerEvent::Started, this, &UGSPAbilitySystemComponent::OnAbilityInputPressed, InputAction).GetHandle();
		}
		else
		{
			UE_LOG(GSPAbility, Warning, TEXT("TryBindAbilityInput: AbilityInputBinding.OnPressedHandle is already set!"));
		}

		// Released event
		if (AbilityInputBinding.OnReleasedHandle == 0)
		{
			AbilityInputBinding.OnReleasedHandle = EIC->BindAction(InputAction, ETriggerEvent::Completed, this, &UGSPAbilitySystemComponent::OnAbilityInputReleased, InputAction).GetHandle();
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


void UGSPAbilitySystemComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	// The AbilitySystemComponent may not have been valid when we first bound input... try again.
	const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
	{
		UE_LOG(GSPAbility, Warning, TEXT("OnAbilityInputPressed: IA: %s ID: %d"), *InputAction->GetName(), FoundBinding->InputID);
		AbilityLocalInputPressed(FoundBinding->InputID);
	}
}

void UGSPAbilitySystemComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
	{
		UE_LOG(GSPAbility, Warning, TEXT("OnAbilityInputReleased: IA: %s ID: %d"), *InputAction->GetName(), FoundBinding->InputID);
		AbilityLocalInputReleased(FoundBinding->InputID);
	}
}

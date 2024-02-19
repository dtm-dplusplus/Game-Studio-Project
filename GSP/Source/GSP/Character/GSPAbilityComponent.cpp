// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPAbilityComponent.h"
#include "kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(GSPAbility);

// Sets default values for this component's properties
UGSPAbilityComponent::UGSPAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}




// Called when the game starts
void UGSPAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGSPAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGSPAbilityComponent::UseAbility()
{
	UE_LOG(GSPAbility, Warning, TEXT("UseAbility UGSPAbilityComponent"));
}
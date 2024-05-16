// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPEnemyCharacter.h"

#include "Components/WidgetComponent.h"
#include "GSP/Ability/GSPAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

AGSPEnemyCharacter::AGSPEnemyCharacter(const FObjectInitializer& ObjectInitializer): AGSPCharacter(ObjectInitializer)
{
	NetUpdateFrequency = 100.0f;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;

	_HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	_HealthBarWidgetComponent->SetupAttachment(RootComponent);
	_HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	_HealthBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f));
	_HealthBarWidgetComponent->SetVisibility(false);
}

void AGSPEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(AActor* Hero = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		_HeroCharacterRef = Hero;
	}
	else
	{
		UE_LOG(GSPCharacter, Error, TEXT("No Hero Character found"));
	}
	


	// Create the attribute set, this replicates by default
	// automatically registers the AttributeSet with the _AbilitySystemComponent
	//_AttributeSet = CreateDefaultSubobject<UGSPAttributeSet>(TEXT("AttributeSet"));

	_AttributeSet->SetMaxHealth(health);
	_AttributeSet->SetHealth(_AttributeSet->GetMaxHealth());

}

void AGSPEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_HeroCharacterRef)
	{
		_DistanceToPlayer = FVector::Dist(GetActorLocation(), _HeroCharacterRef->GetActorLocation());
		_bPLayerInRange= _DistanceToPlayer < _HealthBarRange;
	}

	_HealthBarWidgetComponent->SetVisibility(_bPLayerInRange);
}
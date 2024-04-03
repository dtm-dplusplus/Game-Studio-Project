// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPProjectile.h"

#include "AbilitySystemComponent.h"
#include "../Character/GSPCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GSP/Character/GSPPlayerState.h"

DEFINE_LOG_CATEGORY(GSPWeapon)

// Sets default values
AGSPProjectile::AGSPProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_DebugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
	_DebugMesh->GetRelativeScale3D().Set(0.1f, 0.1f, 0.1f);
	_DebugMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = _DebugMesh;

	_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	_ProjectileMovementComponent->InitialSpeed = 3000.0f;
	_ProjectileMovementComponent->MaxSpeed = 3500.0f;

	_NiagaraMovementFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("_NiagaraMovementFX"));
	_NiagaraMovementFX->SetupAttachment(_DebugMesh);

	OnActorHit.AddDynamic(this, &AGSPProjectile::RecieveHit);
}

void AGSPProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (_NiagaraSpawnFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _NiagaraSpawnFX, GetActorLocation());
	}
}


void AGSPProjectile::RecieveHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	if (const AGSPCharacter* Character = Cast<AGSPCharacter>(OtherActor); _DamageEffect && Character)
	{
		UE_LOG(GSPWeapon, Warning, TEXT("Projectile hit %s"), *Character->GetName());

		if(const AGSPPlayerState* PS = Character->GetGSPPlayerState())
		{
			if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
			{
				const FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();

				UE_LOG(GSPWeapon, Warning, TEXT("Applying damage effect to %s"), *Character->GetName());
				ASC->BP_ApplyGameplayEffectToSelf(_DamageEffect, 0.0, EffectContext);
			}
		}
	}

	if (_NiagaraHitFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _NiagaraHitFX, Hit.ImpactPoint);
	}

	Destroy();
}

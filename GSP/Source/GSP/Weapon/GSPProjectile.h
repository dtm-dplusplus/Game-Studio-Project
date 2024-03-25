// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSPProjectile.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UGameplayEffect;

DECLARE_LOG_CATEGORY_EXTERN(GSPWeapon, Log, All);


UCLASS()
class GSP_API AGSPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGSPProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	void RecieveHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:

	// Debug Mesh used for visualization
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Weapon")
	TObjectPtr<UStaticMeshComponent> _DebugMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Weapon")
	TObjectPtr<class UProjectileMovementComponent> _ProjectileMovementComponent;

	// Particle System that controls active Niagara FX when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Weapon")
	TObjectPtr<UNiagaraComponent> _NiagaraMovementFX;

	// Particle System Played when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Weapon")
	TObjectPtr<UNiagaraSystem> _NiagaraSpawnFX;

	// Particle System that controls active Niagara FX when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Weapon")
	TObjectPtr<UNiagaraSystem> _NiagaraHitFX;

	// Gameplay Damage effect applied when hitting an object
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSP|Weapon")
	TSubclassOf<UGameplayEffect> _DamageEffect;
};

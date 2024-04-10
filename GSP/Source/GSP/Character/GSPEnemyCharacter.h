// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSPCharacter.h"
#include "GSPEnemyCharacter.generated.h"

class UWidgetComponent;
/** Enemy Character class
 * Inherits AbilitySystemComponent from AGSPCharacter
 */
UCLASS()
class GSP_API AGSPEnemyCharacter : public AGSPCharacter
{
	GENERATED_BODY()
public:
	AGSPEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|EnemyCharacter|UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> _HealthBarWidgetComponent;

	/** Health bar range is the distance from player where healt bar is visible */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GSP|EnemyCharacter", meta = (AllowPrivateAccess = "true"))
	float _HealthBarRange = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|EnemyCharacter", meta = (AllowPrivateAccess = "true"))
	bool _bPLayerInRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|EnemyCharacter", meta = (AllowPrivateAccess = "true"))
	float _DistanceToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|EnemyCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> _HeroCharacterRef;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags/Classes/GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "GSPTaggedActor.generated.h"

UCLASS()
class GSP_API AGSPTaggedActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	AGSPTaggedActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()):
	Super(ObjectInitializer)
	{
	}

	/** IGameplayTagAssetInterface */
	UFUNCTION(BlueprintCallable, Category = "GSP|GameplayTags")
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer = _ActorGameplayTags;
	}

protected:
	// Gameplay-related tags associated with this actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor)
	FGameplayTagContainer _ActorGameplayTags;
};
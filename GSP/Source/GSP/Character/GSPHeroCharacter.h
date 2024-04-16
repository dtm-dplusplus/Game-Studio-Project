// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSPCharacter.h"
#include "GSPHeroCharacter.generated.h"

/** Hero Character class
 * Inherits AbilitySystemComponent from AGSPCharacter
 */
UCLASS()
class GSP_API AGSPHeroCharacter : public AGSPCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GSP|GSPCharacter|Input")
	UEnhancedInputComponent* GetEnhancedInputComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|GSPCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> _MappingContext;
	
};

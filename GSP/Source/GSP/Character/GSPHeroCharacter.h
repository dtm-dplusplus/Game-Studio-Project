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

	AGSPHeroCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GSP|GSPHeroCharacter|Input")
	UEnhancedInputComponent* GetEnhancedInputComponent() const;

	void AbilityOne();
	void AbilityTwo();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilityOne();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilityTwo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|GSPHeroCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> _MappingContext;

	/** Input Config can be used to query abilities and their type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|GSPHeroCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGSPInputConfig> _InputConfig; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|GSPHeroCharacter|Ability", meta = (AllowPrivateAccess = "true"))
	FGameplayTag _AbilityOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|GSPHeroCharacter|Ability", meta = (AllowPrivateAccess = "true"))
	FGameplayTag _AbilityTwo;
};

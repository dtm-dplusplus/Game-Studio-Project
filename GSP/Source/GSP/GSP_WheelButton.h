// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "AbilitySystemComponent.h"
#include "GSP_WheelButton.generated.h"
UCLASS()
class GSP_API UGSP_WheelButton : public UCommonButtonBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSP|Ability")
	FGameplayTag Button_Tag;
};

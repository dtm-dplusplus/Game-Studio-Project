// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "GSP_WheelButton.generated.h"
class UInputAction;
UCLASS()
class GSP_API UGSP_WheelButton : public UCommonButtonBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSP|Ability", meta = (AllowPrivateAccess = "true"))
	UInputAction* _action;
};

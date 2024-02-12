// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPPawn.h"

// Sets default values
AGSPPawn::AGSPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGSPPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGSPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGSPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


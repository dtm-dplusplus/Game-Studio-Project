// Fill out your copyright notice in the Description page of Project Settings.


#include "GSPHealthSet.h"

UGSPHealthSet::UGSPHealthSet(): _Health{100.f}, _MaxHealth{ 100.f }
{
	bOutOfHealth = false;
}
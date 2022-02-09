// Fill out your copyright notice in the Description page of Project Settings.


#include "TAbilityShowInfo.h"

// Sets default values for this component's properties
UTAbilityShowInfo::UTAbilityShowInfo()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UTAbilityShowInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTAbilityShowInfo::Show(ATESTCharacter* ObserverCharacter)
{
	OnChangeInfoState.Broadcast(ObserverCharacter, true);
}

void UTAbilityShowInfo::Hide()
{
	OnChangeInfoState.Broadcast(nullptr, false);
}



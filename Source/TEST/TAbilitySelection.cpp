// Fill out your copyright notice in the Description page of Project Settings.


#include "TAbilitySelection.h"

// Sets default values for this component's properties
UTAbilitySelection::UTAbilitySelection()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UTAbilitySelection::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTAbilitySelection::ShowInfo(ATESTCharacter* ObserverCharacter) const
{
	OnChangeSelection.Broadcast(ObserverCharacter, true);
}

void UTAbilitySelection::HideInfo() const
{
	OnChangeSelection.Broadcast(nullptr, false);
}



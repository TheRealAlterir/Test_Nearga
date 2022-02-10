// Fill out your copyright notice in the Description page of Project Settings.


#include "TAbilityInteraction.h"

// Sets default values for this component's properties
UTAbilityInteraction::UTAbilityInteraction()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UTAbilityInteraction::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UTAbilityInteraction::Use(AActor* Source) const
{
	// In case object used by player itself
	ATESTCharacter* Character = Cast<ATESTCharacter>(Source);
	if (Character)
	{
		OnInteraction.Broadcast(Character);
	}

	// Other cases...
	
}

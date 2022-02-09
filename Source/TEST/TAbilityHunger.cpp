// Fill out your copyright notice in the Description page of Project Settings.


#include "TAbilityHunger.h"

// Sets default values for this component's properties
UTAbilityHunger::UTAbilityHunger():
StarvationPower(10.f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.f;
	
}


// Called when the game starts
void UTAbilityHunger::BeginPlay()
{
	Super::BeginPlay();

	CurrentHunger = MaxHunger * 0.5f;
}

// Called every frame
void UTAbilityHunger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Starve(StarvationPower);
}

void UTAbilityHunger::Eat(const float FoodValue)
{
	CurrentHunger += FoodValue;
	if (CurrentHunger > MaxHunger)
	{
		CurrentHunger = MaxHunger;
	}

}

void UTAbilityHunger::Starve(const float HungerValue)
{
	CurrentHunger -= HungerValue;
	if (CurrentHunger < 0.f)
	{
		CurrentHunger = 0.f;
		
		// Starvation
		OnStarvation.Broadcast();
	}
	
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TAbilityHunger.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UTAbilityHunger::UTAbilityHunger():
StarvationPower(10.f),
StarvationDamage(5.f)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UTAbilityHunger::BeginPlay()
{
	Super::BeginPlay();

	CurrentHunger = MaxHunger * 0.5f;
}

void UTAbilityHunger::Eat(const float FoodValue)
{
	CurrentHunger += FoodValue;
	if (CurrentHunger > MaxHunger)
	{
		CurrentHunger = MaxHunger;
	}

}

void UTAbilityHunger::Starve()
{
	CurrentHunger -= StarvationPower;
	if (CurrentHunger < 0.f)
	{
		CurrentHunger = 0.f;
		
		// Starvation
		OnStarvation.Broadcast();
	}
	
}

void UTAbilityHunger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTAbilityHunger, CurrentHunger)
}


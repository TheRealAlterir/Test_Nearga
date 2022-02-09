// Fill out your copyright notice in the Description page of Project Settings.


#include "TAbilityHealth.h"

// Sets default values for this component's properties
UTAbilityHealth::UTAbilityHealth():
bCanBeKilled(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

// Called when the game starts
void UTAbilityHealth::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UTAbilityHealth::Heal(const float HealingApplied)
{
	CurrentHealth += HealingApplied;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	OnChangeHealth.Broadcast(CurrentHealth);
}

void UTAbilityHealth::DealSingleDamage(const float DamageApplied)
{
	CurrentHealth -= DamageApplied;

	if (CurrentHealth <= 0.f && !bCanBeKilled)
	{
		CurrentHealth = 1.f;
	}
	else
	{
		//....
	}

	OnChangeHealth.Broadcast(CurrentHealth);
} 


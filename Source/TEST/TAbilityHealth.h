// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TAbilityHealth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHealthDelegate, float, CurrentHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST_API UTAbilityHealth : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTAbilityHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called on healing. May be animated like dot healing effect
	UFUNCTION(BlueprintCallable)
	void Heal(const float HealingApplied);

	// Called on dealing single shot damage
	UFUNCTION(BlueprintCallable)
	void DealSingleDamage(const float DamageApplied);

public:
	// Triggered on Health change
	UPROPERTY(BlueprintAssignable)
	FChangeHealthDelegate OnChangeHealth;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	// false by default as there are no task for deaths.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeKilled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHealth; 
	
};

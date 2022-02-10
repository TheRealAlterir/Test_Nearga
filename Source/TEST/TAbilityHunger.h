// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TAbilityHunger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStarvationDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST_API UTAbilityHunger : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTAbilityHunger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Called when actor eat something
	UFUNCTION(BlueprintCallable)
	void Eat(const float FoodValue);

	// Should be called on tick, but also may be called on eating bad food
	UFUNCTION(BlueprintCallable)
	void Starve(const float HungerValue);

public:
	UPROPERTY(BlueprintAssignable)
	FOnStarvationDelegate OnStarvation;


protected:
	// Max value for hunger stat.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHunger;

	// Takes selected amount of Hunger every tick.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StarvationPower;

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float CurrentHunger;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

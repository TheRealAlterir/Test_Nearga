// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TESTCharacter.h"
#include "Components/ActorComponent.h"
#include "TAbilityInteraction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, ATESTCharacter*, Source);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST_API UTAbilityInteraction : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTAbilityInteraction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Call on pressing Use button.
	UFUNCTION(BlueprintCallable)
	void Use(AActor* Source) const;

public:
	UPROPERTY(BlueprintAssignable)
	FInteractionDelegate OnInteraction;
	
};

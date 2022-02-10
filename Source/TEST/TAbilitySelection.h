// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TESTCharacter.h"
#include "Components/ActorComponent.h"
#include "TAbilitySelection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeInfoState, ATESTCharacter*, Observer, bool, State);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UTAbilitySelection : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTAbilitySelection();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called when something selected
	UFUNCTION(BlueprintCallable)
	void ShowInfo(ATESTCharacter* ObserverCharacter) const;

	// Called after losing selection
	UFUNCTION(BlueprintCallable)
	void HideInfo() const;

public:
	// Triggered on changing selection state
	UPROPERTY(BlueprintAssignable)
	FChangeInfoState OnChangeSelection;
		
};

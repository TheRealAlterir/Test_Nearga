// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TESTCharacter.h"
#include "Components/ActorComponent.h"
#include "Modules/ModuleObserver.h"
#include "TAbilityShowInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeInfoState, ATESTCharacter*, Observer, bool, bShow);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UTAbilityShowInfo : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTAbilityShowInfo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called when something selected
	UFUNCTION(BlueprintCallable)
	void Show(ATESTCharacter* ObserverCharacter);

	// Called after losing selection
	UFUNCTION(BlueprintCallable)
	void Hide();

public:
	// Triggered on Health change
	UPROPERTY(BlueprintAssignable)
	FChangeInfoState OnChangeInfoState;
		
};

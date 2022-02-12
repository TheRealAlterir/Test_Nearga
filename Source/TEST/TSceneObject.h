// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSceneObject.generated.h"

UCLASS()
class TEST_API ATSceneObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATSceneObject();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InfoWidget;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UTAbilitySelection* SelectionAbility;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	virtual void OnChangeSelection(class ATESTCharacter* ObserverCharacter, bool State);

	UFUNCTION()
	virtual void ShowInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATESTCharacter* Observer;

private:
	bool bShowSelfInfo;

protected:
	/** Returns InfoWidget subobject **/
	FORCEINLINE class UWidgetComponent* GetInfoWidget() const { return InfoWidget; }
};

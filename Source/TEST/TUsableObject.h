// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TSceneObject.h"
#include "TUsableObject.generated.h"

UENUM(BlueprintType)
enum EAppliedEffect
{
	Healing,
	Hunger,

	//...

	None
};

USTRUCT(BlueprintType)
struct FConsumableEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAppliedEffect> Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};

UCLASS()
class TEST_API ATUsableObject : public ATSceneObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATUsableObject();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SelectionCollision;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UTAbilityInteraction* InteractionAbility;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	virtual void OnInteraction(ATESTCharacter* Source);

protected:
	// Allow to create multi-effect items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<FConsumableEffect> Effects;
};

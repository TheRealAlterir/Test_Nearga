// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
class TEST_API ATUsableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATUsableObject();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InfoWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SelectionCollision;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UTAbilitySelection* SelectionAbility;

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
	void OnChangeSelection(class ATESTCharacter* ObserverCharacter, bool State);

	UFUNCTION()
	void OnInteraction(ATESTCharacter* Source);

	UFUNCTION()
	void ShowInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATESTCharacter* Observer;

	// Allow to create multi-effect items
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConsumableEffect> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowSelfInfo;

	/** Returns InfoWidget subobject **/
	FORCEINLINE class UWidgetComponent* GetInfoWidget() const { return InfoWidget; }
	
};

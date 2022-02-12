// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TESTCharacter.generated.h"

UCLASS(config=Game)
class ATESTCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* MeleeCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Viewpoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Object, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InfoWidget;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UTAbilityHealth* HealthAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UTAbilityHunger* HungerAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UTAbilitySelection* SelectionAbility;

public:
	ATESTCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	FTimerHandle TimerDelegate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

protected:
	UFUNCTION()
	AActor* GetRaycastedObject(FVector Destination) const;

	UFUNCTION()
	void ViewSelection();

	UFUNCTION(BlueprintNativeEvent)
	void ShowInfo();
	
	UFUNCTION()
	void OnChangeHealth(float CurrentHealth);

	UFUNCTION()
	void OnStarvation();

	UFUNCTION()
	void OnChangeSelection(class ATESTCharacter* ObserverCharacter, bool State);

	UFUNCTION()
	void OnInteraction();

private:
	void Interact(const FVector Destination = FVector(0.f, 0.f, 0.f));

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnInteraction(FVector Destination);
	bool Server_OnInteraction_Validate(FVector Destination);
	void Server_OnInteraction_Implementation(FVector Destination);

	UFUNCTION()
	void OnAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnAttack();
	bool Server_OnAttack_Validate();
	void Server_OnAttack_Implementation();

private:
	bool CheckFocus(AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* SelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATESTCharacter* Observer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	UAnimMontage* Montage;

private:
	// Used for timer to detect how long object is viewed
	int32 FocusTime;
	
	bool bShowSelfInfo;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Viewpoint subobject **/
	FORCEINLINE class USceneComponent* GetViewpoint() const { return Viewpoint; }
	/** Returns InfoWidget subobject **/
	FORCEINLINE class UWidgetComponent* GetInfoWidget() const { return InfoWidget; }
};

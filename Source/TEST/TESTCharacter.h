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
	void ViewSelection();

	UFUNCTION()
	void OnChangeHealth(float CurrentHealth);

	UFUNCTION()
	void OnStarvation();

	UFUNCTION()
	void OnChangeSelection(class ATESTCharacter* ObserverCharacter, bool State);

	void OnInteraction();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnInteraction(class ATUsableObject* Object);
	bool Server_OnInteraction_Validate(ATUsableObject* Object);
	void Server_OnInteraction_Implementation(ATUsableObject* Object);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_OnInteraction(ATUsableObject* Object);
	bool Multi_OnInteraction_Validate(ATUsableObject* Object);
	void Multi_OnInteraction_Implementation(ATUsableObject* Object);

	void OnAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnAttack();
	bool Server_OnAttack_Validate();
	void Server_OnAttack_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_OnAttack();
	bool Multi_OnAttack_Validate();
	void Multi_OnAttack_Implementation();

private:
	bool CheckFocus(AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* SelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATESTCharacter* Observer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowSelfInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;

private:
	// Used for timer to detect how long object is viewed
	int32 FocusTime;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns Viewpoint subobject **/
	FORCEINLINE class USceneComponent* GetViewpoint() const { return Viewpoint; }
	
};

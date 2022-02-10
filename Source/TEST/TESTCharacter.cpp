// Copyright Epic Games, Inc. All Rights Reserved.

#include "TESTCharacter.h"
#include "TAbilityHealth.h"
#include "TAbilityHunger.h"
#include "TAbilityInteraction.h"
#include "TAbilitySelection.h"
#include "TUsableObject.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ATESTCharacter

ATESTCharacter::ATESTCharacter():
	FocusTime(0)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoWidget"));
	InfoWidget->SetupAttachment(RootComponent);

	HealthAbility = CreateDefaultSubobject<UTAbilityHealth>(TEXT("HealthAbility"));
	AddOwnedComponent(HealthAbility);
	HungerAbility = CreateDefaultSubobject<UTAbilityHunger>(TEXT("HungerAbility"));
	AddOwnedComponent(HungerAbility);
	SelectionAbility = CreateDefaultSubobject<UTAbilitySelection>(TEXT("SelectionAbility"));
	AddOwnedComponent(SelectionAbility);

	HealthAbility->OnChangeHealth.AddDynamic(this, &ATESTCharacter::OnChangeHealth);
	HungerAbility->OnStarvation.AddDynamic(this, &ATESTCharacter::OnStarvation);
	SelectionAbility->OnChangeSelection.AddDynamic(this, &ATESTCharacter::OnChangeSelection);
}

void ATESTCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Check is Character controlled by this Player to avoid creating selection from both players.
	if (this->GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// Check objects in view. This is high-cost function fired every x sec.
		GetWorldTimerManager().SetTimer(TimerDelegate, this, &ATESTCharacter::ViewSelection, 0.25f, true);
	}
}

// Called every frame
void ATESTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATESTCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATESTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATESTCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ATESTCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATESTCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATESTCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATESTCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//////////////////////////

void ATESTCharacter::ViewSelection()
{
	FHitResult HitResult;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);

	FVector Start = GetMesh()->GetSocketLocation("head");
	FVector End = GetFollowCamera()->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(
		GetFollowCamera()->GetComponentRotation()) * 1200.f + FVector(0.f, 0.f, 200.f);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParameters);

	if (SelectedActor)
	{
		// Trying to select object. This function prevent nesting
		if (CheckFocus(HitResult.Actor.Get()))
		{
			return;
		}

		// Triggered if changed focus.
		UTAbilitySelection* Ability = SelectedActor->FindComponentByClass<UTAbilitySelection>();
		if (Ability)
		{
			SelectedActor->FindComponentByClass<UTAbilitySelection>()->HideInfo();
		}
	}

	if (HitResult.Actor != nullptr)
	{
		// Focus on new object
		SelectedActor = HitResult.Actor.Get();
	}
	FocusTime = 0;
}

void ATESTCharacter::OnChangeHealth(float CurrentHealth)
{
	//...
}

void ATESTCharacter::OnStarvation()
{
	HealthAbility->DealSingleDamage(5.f);
}

void ATESTCharacter::OnChangeSelection(class ATESTCharacter* ObserverCharacter, bool State)
{
	Observer = ObserverCharacter;
	bShowSelfInfo = State;
	InfoWidget->SetVisibility(State);
	
}

bool ATESTCharacter::CheckFocus(AActor* Actor)
{
	// Actor gets selection after being watched for 2 consecutive ticks
	if (Actor == SelectedActor && Actor != nullptr)
	{
		const int32 TicksToSelect = 1;
		if (FocusTime < TicksToSelect)
		{
			++FocusTime;
		}
		else if (FocusTime == TicksToSelect)
		{
			// Prevent from counting
			++FocusTime;

			// Destroy and create new widget
			UTAbilitySelection* Ability = SelectedActor->FindComponentByClass<UTAbilitySelection>();
			if (Ability)
			{
				Ability->ShowInfo(this);
			}
		}

		return true;
	}

	return false;
}


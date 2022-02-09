// Copyright Epic Games, Inc. All Rights Reserved.

#include "TESTCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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
SelectionProgress(0)
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
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
}

void ATESTCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Check is Character controlled by this Player to avoid creating selection from both players.
	if (this->GetController() ==  UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// Check objects in view. This is high-cost function fired every x sec.
		GetWorldTimerManager().SetTimer(TimerDelegate, this, &ATESTCharacter::ViewSelection, 0.25f, true);
	}
	
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
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
	TArray<FHitResult> HitResult;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this); 
	
	FVector Start = GetMesh()->GetSocketLocation("head");
	FVector End = GetFollowCamera()->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(GetFollowCamera()->GetComponentRotation()) * 2000.f;

	GetWorld()->LineTraceMultiByChannel(HitResult, Start, End, ECollisionChannel::ECC_Camera, CollisionParameters);
	
	for (auto Hit : HitResult)
	{
		// Actor gets selection after being watched for 3 consecutive ticks
		if (Hit.Actor == SelectedActor)
		{
			// Should be magic. TL to write this setting
			if (SelectionProgress < 2) 
			{
				++SelectionProgress;
			}
			else if(SelectionProgress == 2)
			{
				// Prevent from firing
				++SelectionProgress;
				// Destroy and create new widget
				SetNewInfoWidget(ESelectedType::Character);
			}
			
			return;
		}
		
		ATESTCharacter* Character = Cast<ATESTCharacter>(Hit.Actor);
		if (Character)
		{
			SelectedActor = Character;
			SelectionProgress = 0;
			
			return;
		}

		// cast to object...
	}

	// Triggered if nothing is found.
	SetNewInfoWidget(ESelectedType::None);
	SelectedActor = nullptr;
	SelectionProgress = 0;
	
}

void ATESTCharacter::SetNewInfoWidget_Implementation(ESelectedType Type)
{
	
}


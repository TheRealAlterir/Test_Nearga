// Fill out your copyright notice in the Description page of Project Settings.


#include "TUsableObject.h"

#include "TAbilityHealth.h"
#include "TAbilityHunger.h"
#include "TAbilityInteraction.h"
#include "TAbilitySelection.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATUsableObject::ATUsableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
	SetReplicates(true);

	SelectionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SelectionCollision"));
	SelectionCollision->SetupAttachment(RootComponent);
	SelectionCollision->InitSphereRadius(200.f);
	
	InteractionAbility = CreateDefaultSubobject<UTAbilityInteraction>(TEXT("InteractionAbility"));
	AddOwnedComponent(InteractionAbility);
	
	InteractionAbility->OnInteraction.AddDynamic(this, &ATUsableObject::OnInteraction);
}

// Called when the game starts or when spawned
void ATUsableObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATUsableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATUsableObject::OnInteraction(ATESTCharacter* Source)
{
	for (auto Effect : Effects)
	{
		switch (Effect.Effect)
		{
		case Healing:
			Source->HealthAbility->Heal(Effect.Value);
			break;
		case Hunger:
			Source->HungerAbility->Eat(Effect.Value);
			break;

		default:
			break;
		}
	}

	this->Destroy();
}


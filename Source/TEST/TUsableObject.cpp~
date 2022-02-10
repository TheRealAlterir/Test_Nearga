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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoWidget"));
	InfoWidget->SetupAttachment(RootComponent);

	SelectionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SelectionCollision"));
	SelectionCollision->SetupAttachment(RootComponent);
	SelectionCollision->InitSphereRadius(200.f);

	SelectionAbility = CreateDefaultSubobject<UTAbilitySelection>(TEXT("SelectionAbility"));
	AddOwnedComponent(SelectionAbility);
	InteractionAbility = CreateDefaultSubobject<UTAbilityInteraction>(TEXT("InteractionAbility"));
	AddOwnedComponent(InteractionAbility);

	SelectionAbility->OnChangeSelection.AddDynamic(this, &ATUsableObject::OnChangeSelection);
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

	if (bShowSelfInfo)
	{
		ShowInfo();
	}
}

void ATUsableObject::OnChangeSelection(ATESTCharacter* ObserverCharacter, bool State)
{
	Observer = ObserverCharacter;
	bShowSelfInfo = State;
	InfoWidget->SetVisibility(State);
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

void ATUsableObject::ShowInfo()
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetInfoWidget()->GetComponentLocation(), Observer->GetInfoWidget()->GetComponentLocation());

	GetInfoWidget()->SetWorldRotation(Rot);
}


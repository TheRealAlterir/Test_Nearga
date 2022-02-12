// Fill out your copyright notice in the Description page of Project Settings.


#include "TSceneObject.h"

#include "TAbilitySelection.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATSceneObject::ATSceneObject()
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

	SelectionAbility = CreateDefaultSubobject<UTAbilitySelection>(TEXT("SelectionAbility"));
	AddOwnedComponent(SelectionAbility);
	SelectionAbility->OnChangeSelection.AddDynamic(this, &ATSceneObject::OnChangeSelection);

}

// Called when the game starts or when spawned
void ATSceneObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATSceneObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShowSelfInfo)
	{
		ShowInfo();
	}
}

void ATSceneObject::OnChangeSelection(ATESTCharacter* ObserverCharacter, bool State)
{
	Observer = ObserverCharacter;
	bShowSelfInfo = State;
	InfoWidget->SetVisibility(State);
}

void ATSceneObject::ShowInfo()
{
	const FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetInfoWidget()->GetComponentLocation(), Observer->GetInfoWidget()->GetComponentLocation());

	GetInfoWidget()->SetWorldRotation(Rot);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TSceneObject.h"

// Sets default values
ATSceneObject::ATSceneObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

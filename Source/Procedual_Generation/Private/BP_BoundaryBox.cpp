// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_BoundaryBox.h"

// Sets default values
ABP_BoundaryBox::ABP_BoundaryBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_BoundaryBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_BoundaryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


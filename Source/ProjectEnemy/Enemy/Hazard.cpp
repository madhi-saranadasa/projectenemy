// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"

AHazard::AHazard()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AHazard::BeginPlay()
{
	Super::BeginPlay();
	
}


void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
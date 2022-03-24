// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayManager.h"
#include "MyPlayerController.h"
#include "MyCamera.h"


UDisplayManager::UDisplayManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UDisplayManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterPlayerController();
	SpawnCamera();
}


void UDisplayManager::RegisterPlayerController()
{
	OwningPC = Cast<AMyPlayerController>(GetOwner());

	if (!OwningPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to register owning player controller!"));
	}
}


void UDisplayManager::SpawnCamera()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (CameraPawnAsset)
	{
		AMyCamera* MyCameraDefault = CameraPawnAsset.GetDefaultObject();

		MyCamera = GetWorld()->SpawnActor<AMyCamera>(CameraPawnAsset, MyCameraDefault->StartLocation, MyCameraDefault->StartRotation, SpawnInfo);

		OwningPC->SetViewTarget(MyCamera);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera is not assigned on display manager"));
	}
}


void UDisplayManager::ResetView()
{
	OwningPC->SetViewTarget(MyCamera);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "DisplayManager.h"
#include <DrawDebugHelpers.h>
#include <Components/CapsuleComponent.h>
#include "../Player/PlayerCharacter.h"


AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	//bAutoManageActiveCameraTarget = false; // prevents resetting view after possession

	// Create display manager
	//DisplayManager = CreateDefaultSubobject<UDisplayManager>(TEXT("DisplayManager"));
}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

}


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AMyPlayerController::OnSpaceBarPress);
}


void AMyPlayerController::OnSpaceBarPress()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to spawn player"));
	SpawnPlayerOnGround();
}


void AMyPlayerController::SpawnPlayerOnGround()
{

	if (!PlayerCharacterAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn failed, no player asset is assigned"));
		return;
	}
	else if (CurrentPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player pawn already exists"));
		return;
	}

	// Get the surface point
	FHitResult Hit;
	FVector TraceStart = FVector(0.0f, 0.0f, 100.0f);
	FVector TraceEnd = FVector(0.0f, 0.0f, -100.0f);
	FCollisionQueryParams TraceParams;

	bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	if (isHit)
	{

		APlayerCharacter* PlayerDefault = PlayerCharacterAsset.GetDefaultObject();
		FVector StartPosition = Hit.ImpactPoint + FVector(0.0f, 0.0f, PlayerDefault->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		CurrentPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterAsset, StartPosition, FRotator::ZeroRotator, SpawnInfo);

		Possess(CurrentPlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing was hit, can't place player"));
	}
}

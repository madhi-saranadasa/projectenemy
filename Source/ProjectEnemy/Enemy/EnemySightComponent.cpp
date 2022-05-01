// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySightComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../Player/PlayerCharacter.h"


UEnemySightComponent::UEnemySightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default sight settings
	SightInterval = 0.1f;
	SightRadius = 200.0f;
	SightOffset = FVector(100.0f, 0.0f, 0.0f);
}


void UEnemySightComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SightIntervalHandle, this, &UEnemySightComponent::SightCheck, SightInterval, true);
}


void UEnemySightComponent::SightCheck()
{
	SphereTraceMethod();
}

void UEnemySightComponent::SphereTraceMethod()
{
	// Find start and end
	FVector SightStart = GetOwner()->GetActorLocation();
	FVector SightEnd = SightStart + GetOwner()->GetActorForwardVector() * SightOffset.X + GetOwner()->GetActorRightVector() * SightOffset.Y;

	// Array to store results
	TArray<FHitResult> HitArray;

	// Trace parameters
	TArray<TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	// Do the trace
	bool bAnyHits = UKismetSystemLibrary::SphereTraceMultiForObjects(this, SightStart, SightEnd, SightRadius,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Red, 0.1f);

	if (bAnyHits)
	{
		for (FHitResult Hit : HitArray)
		{
			AActor* HitActor = Hit.GetActor();

			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Hit.GetActor());

			if (PlayerCharacter)
			{
				SightResponse.Broadcast(PlayerCharacter);
			}
		}
	}
}

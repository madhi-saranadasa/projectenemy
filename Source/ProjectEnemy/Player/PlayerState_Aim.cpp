// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Aim.h"
#include <Kismet/KismetMathLibrary.h>

UPlayerState_Aim::UPlayerState_Aim()
{
	StateName = EPlayerStateName::AIM;
}


void UPlayerState_Aim::OnStateEnter()
{
	UE_LOG(LogTemp, Warning, TEXT("Entered aim state"));
	OwningCharacter->UpdateMoveCompParameters(100.0f, 1000.0f, false);
}


void UPlayerState_Aim::StateTick(float DeltaTime)
{
	// Find current mouse position
	FVector MousePosition = OwningCharacter->GetMousePosition();
	MousePosition.Z = OwningCharacter->GetActorLocation().Z;

	// Calculate look rotation
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwningCharacter->GetActorLocation(), MousePosition);
	// Calculate new rotation based on turn speed and the difference between current and target rotation
	FRotator NewRotation = FMath::RInterpTo(OwningCharacter->GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
	// Update rotation
	OwningCharacter->SetActorRotation(NewRotation);
}


void UPlayerState_Aim::OnStateExit()
{
	UE_LOG(LogTemp, Warning, TEXT("Left aim state"));
	OwningCharacter->UpdateMoveCompParameters(400.0f, 600.0f, true);
}

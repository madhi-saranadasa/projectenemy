// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Aim.h"
#include <Kismet/KismetMathLibrary.h>

UPlayerState_Aim::UPlayerState_Aim()
{
	StateName = EPlayerStateName::AIM;
}


void UPlayerState_Aim::OnStateEnter()
{
	// Play charging particles
	OwningCharacter->ToggleChargeParticle(true);

	// Update movement parameters
	OwningCharacter->UpdateMoveCompParameters(200.0f, 1000.0f, false);

	// If previous state was dash, then keep going with timer/charge reset
	if (StateMachine->PreviousState == EPlayerStateName::DASH)
	{
		return;
	}

	// Set both timers
	GetWorld()->GetTimerManager().SetTimer(PrimaryTimerHandle, this, &UPlayerState_Aim::PrimaryChargeReady, PrimaryDuration, false);
	GetWorld()->GetTimerManager().SetTimer(SecondaryTimerHandle, this, &UPlayerState_Aim::SecondaryChargeReady, SecondaryDuration, false);
	// Clear charges
	StateMachine->bChargePrimary = false;
	StateMachine->bChargeSecondary = false;
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
	Super::OnStateExit();

	// Stop charging particles
	OwningCharacter->ToggleChargeParticle(false);

	// Unless we are going into dash, clear timers and clear charges
	if (StateMachine->NextState != EPlayerStateName::DASH)
	{
		GetWorld()->GetTimerManager().ClearTimer(PrimaryTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(SecondaryTimerHandle);

		//StateMachine->bChargePrimary = false;
		//StateMachine->bChargeSecondary = false;
	}
}


void UPlayerState_Aim::PrimaryChargeReady()
{
	StateMachine->bChargePrimary = true;
	OwningCharacter->PlayPrimaryFlash();
}


void UPlayerState_Aim::SecondaryChargeReady()
{
	StateMachine->bChargeSecondary = true;
	OwningCharacter->PlaySecondaryFlash();
}

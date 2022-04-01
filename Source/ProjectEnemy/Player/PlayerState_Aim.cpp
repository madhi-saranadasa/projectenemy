// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Aim.h"
#include <Kismet/KismetMathLibrary.h>

UPlayerState_Aim::UPlayerState_Aim()
{
	StateName = EPlayerStateName::AIM;
}


void UPlayerState_Aim::OnStateEnter()
{
	// Update movement parameters
	OwningCharacter->UpdateMoveCompParameters(100.0f, 1000.0f, false);
	
	// If timer is active or a charge is ready, just keep going
	if (GetWorld()->GetTimerManager().IsTimerActive(ChargeTimerHandle) | StateMachine->bChargeReady)
	{
		return;
	}

	// If a timer is not going and there is no charge ready, then start charge sequence
	// Start timer
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UPlayerState_Aim::OnChargeReady, ChargeDuration, false);
	// Set bool to false
	StateMachine->SetChargeReady(false);
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

	// Unless we are going into dash, clear the timer and clear the charge
	if (StateMachine->NextState != EPlayerStateName::DASH)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
		StateMachine->SetChargeReady(false);
	}

	OwningCharacter->UpdateMoveCompParameters(400.0f, 600.0f, true);
}


void UPlayerState_Aim::OnChargeReady()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack ready!"));
	StateMachine->SetChargeReady(true);
}

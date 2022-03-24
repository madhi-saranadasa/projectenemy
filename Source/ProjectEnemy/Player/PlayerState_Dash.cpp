// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Dash.h"
#include "PlayerCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>


UPlayerState_Dash::UPlayerState_Dash()
{
	StateName = EPlayerStateName::DASH;
}


void UPlayerState_Dash::OnStateEnter()
{
	// Update state machine
	StateMachine->SetCanDash(false);

	// Update player character: limit input, store dash vector, start animation
	OwningCharacter->GetController()->SetIgnoreMoveInput(true);
	DashVector = OwningCharacter->GetLastMovementInputVector().GetSafeNormal();
	OwningCharacter->StartMontage(DashAnim);

	// Spawn initial particles
	UNiagaraFunctionLibrary::SpawnSystemAttached(DashParticles, OwningCharacter->GetRootComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);
	
	// Start timers
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UPlayerState_Dash::OnDashEnd, DashDuration, false);
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &UPlayerState_Dash::OnDashCooldownEnd, DashDuration + AddedDashCooldown, false);
}


void UPlayerState_Dash::StateTick(float DeltaTime)
{
	// Movement
	float AlphaPoint = DashCurve->GetFloatValue(GetWorld()->GetTimerManager().GetTimerElapsed(DashTimerHandle) / DashDuration);
	OwningCharacter->GetCharacterMovement()->MoveSmooth(DashVector * DashSpeed * AlphaPoint, DeltaTime);
}


void UPlayerState_Dash::OnDashEnd()
{
	// Change state after timer expires
	StateMachine->ChangeState(EPlayerStateName::DEFAULT);
}


void UPlayerState_Dash::OnStateExit()
{
	// Allow movement after exiting this state
	OwningCharacter->GetController()->SetIgnoreMoveInput(false);
}


void UPlayerState_Dash::OnDashCooldownEnd()
{
	// Set dash check after second time expires
	StateMachine->SetCanDash(true);
}

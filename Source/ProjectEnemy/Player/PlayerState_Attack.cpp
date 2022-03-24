// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Attack.h"
#include "PlayerCharacter.h"
#include "AttackVolume.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>


UPlayerState_Attack::UPlayerState_Attack()
{
	StateName = EPlayerStateName::ATTACK;
}


void UPlayerState_Attack::OnStateEnter()
{
	// Update state machine
	StateMachine->SetCanAttack(false);
	StateMachine->bAttackMove = true;

	// Update player character: limit input, store Attack vector, start animation
	OwningCharacter->GetController()->SetIgnoreMoveInput(true);
	AttackVector = OwningCharacter->GetActorForwardVector();
	OwningCharacter->StartMontage(AttackAnim);

	//
	UNiagaraFunctionLibrary::SpawnSystemAttached(SlashParticles, OwningCharacter->GetRootComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);

	// Start timers
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UPlayerState_Attack::OnAttackEnd, AttackDuration, false);
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &UPlayerState_Attack::OnAttackCooldownEnd, AttackDuration + AddedAttackCooldown, false);

}


void UPlayerState_Attack::StateTick(float DeltaTime)
{

	float AlphaPoint = AttackCurve->GetFloatValue(GetWorld()->GetTimerManager().GetTimerElapsed(AttackTimerHandle) / AttackDuration);

	// Movement
	if (StateMachine->bAttackMove)
	{

		OwningCharacter->GetCharacterMovement()->MoveSmooth(AttackVector * AttackSpeed * AlphaPoint, DeltaTime);
	}
	else
	{
		OwningCharacter->GetCharacterMovement()->MoveSmooth(AttackVector * AttackSpeed * 1.0f * AlphaPoint, DeltaTime);
	}

	// Activate attack volume
	OwningCharacter->AttackVolume->SweepAttackVolume(DeltaTime*2.0f);

}


void UPlayerState_Attack::OnAttackEnd()
{
	// Change state after timer expires
	StateMachine->ChangeState(EPlayerStateName::DEFAULT);
}


void UPlayerState_Attack::OnStateExit()
{
	// Allow movement after exiting this state
	OwningCharacter->GetController()->SetIgnoreMoveInput(false);
}


void UPlayerState_Attack::OnAttackCooldownEnd()
{
	// Set Attack check after second time expires
	StateMachine->SetCanAttack(true);
}
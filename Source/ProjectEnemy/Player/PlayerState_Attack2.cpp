// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Attack2.h"
#include "PlayerCharacter.h"
#include "AttackVolume.h"
#include <GameFramework/CharacterMovementComponent.h>


UPlayerState_Attack2::UPlayerState_Attack2()
{
	StateName = EPlayerStateName::ATTACK2;
}


void UPlayerState_Attack2::OnStateEnter()
{
	// Update state machine
	StateMachine->bCanAttack = false;
	StateMachine->bAttackLanded = false;

	// Update player character: limit input, store Attack vector, start animation
	OwningCharacter->GetController()->SetIgnoreMoveInput(true);
	AttackVector = OwningCharacter->GetActorForwardVector();
	OwningCharacter->StartMontage(AttackAnim);

	// Play particle effects
	OwningCharacter->PlayAttackParticles(false);

	// Start timers
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UPlayerState_Attack2::OnAttackEnd, AttackDuration, false);
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &UPlayerState_Attack2::OnAttackCooldownEnd, AttackDuration + AddedAttackCooldown, false);

	UE_LOG(LogTemp, Warning, TEXT("Attack2"));
}


void UPlayerState_Attack2::StateTick(float DeltaTime)
{
	float AlphaPoint = AttackCurve->GetFloatValue(GetWorld()->GetTimerManager().GetTimerElapsed(AttackTimerHandle) / AttackDuration);

	// Movement - option to change behavior if the attack landed
	if (!StateMachine->bAttackLanded)
	{

		OwningCharacter->GetCharacterMovement()->MoveSmooth(AttackVector * AttackSpeed * 1.0f * AlphaPoint, DeltaTime);
	}
	else
	{
		OwningCharacter->GetCharacterMovement()->MoveSmooth(AttackVector * AttackSpeed * 0.4f * AlphaPoint, DeltaTime);
	}

	// Activate attack volume
	OwningCharacter->AttackVolume->SweepAttackVolume(DeltaTime * 2.0f);
}


void UPlayerState_Attack2::OnStateExit()
{
	Super::OnStateExit();

	OwningCharacter->GetController()->SetIgnoreMoveInput(false);
}


void UPlayerState_Attack2::OnAttackEnd()
{
	// Change state after timer expires if we are still in the attack state
	if (StateMachine->GetCurrentState() == EPlayerStateName::ATTACK2)
	{
		StateMachine->ChangeState(EPlayerStateName::DEFAULT);
	}
}


void UPlayerState_Attack2::OnAttackCooldownEnd()
{
	// Set Attack check after second time expires
	StateMachine->bCanAttack = true;
}

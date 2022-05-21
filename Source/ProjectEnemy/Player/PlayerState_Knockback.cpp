// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Knockback.h"
#include "PlayerCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>


UPlayerState_Knockback::UPlayerState_Knockback()
{
	StateName = EPlayerStateName::KNOCKBACK;
}


void UPlayerState_Knockback::OnStateEnter()
{
	// Knockback vector should be updated on the blackboard before this

	// Update state machine
	StateMachine->bCanBeHit = false;

	// Update player character: limit input, start animation
	OwningCharacter->GetController()->SetIgnoreMoveInput(true);

	// Start animation
	OwningCharacter->StartMontage(KnockbackAnim);

	// Start hit flash
	OwningCharacter->GetMesh()->SetScalarParameterValueOnMaterials("bHit", 1);

	// Start timers
	GetWorld()->GetTimerManager().SetTimer(KnockbackTimerHandle, this, &UPlayerState_Knockback::OnKnockbackEnd, KnockbackDuration, false);
	GetWorld()->GetTimerManager().SetTimer(KnockbackCooldownTimerHandle, this, &UPlayerState_Knockback::OnKnockbackCooldownEnd, KnockbackDuration + AddedKnockbackCooldown, false);

	UE_LOG(LogTemp, Warning, TEXT("Knockback"));

	
}


void UPlayerState_Knockback::StateTick(float DeltaTime)
{
	// Movement
	float AlphaPoint = KnockbackCurve->GetFloatValue(GetWorld()->GetTimerManager().GetTimerElapsed(KnockbackTimerHandle) / KnockbackDuration);
	OwningCharacter->GetCharacterMovement()->MoveSmooth(StateMachine->KnockbackVector * KnockbackSpeed * AlphaPoint, DeltaTime);
}


void UPlayerState_Knockback::OnKnockbackEnd()
{
	// Change state after timer expires
	StateMachine->ChangeState(EPlayerStateName::DEFAULT);
}


void UPlayerState_Knockback::OnStateExit()
{
	Super::OnStateExit();

	OwningCharacter->GetController()->SetIgnoreMoveInput(false);
}


void UPlayerState_Knockback::OnKnockbackCooldownEnd()
{
	// Set knockback check after second time expires
	StateMachine->bCanBeHit = true;

	// Stop hit flash
	OwningCharacter->GetMesh()->SetScalarParameterValueOnMaterials("bHit", 0);
}

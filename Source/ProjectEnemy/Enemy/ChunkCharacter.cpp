// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "EnemyHealthComponent.h"


AChunkCharacter::AChunkCharacter()
{
	// Capsule component settings
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);

	// Health settings
	HealthComp->MaxHealth = 2;
}

void AChunkCharacter::SightResponse(ACharacter* InstigatorCharacter)
{
	if (IsState(EEnemyStateName::GRAZE))
	{
		EnemyBlackboard->SetValueAsObject("TargetCharacter", InstigatorCharacter);
		ChangeState(EEnemyStateName::PURSUIT);
	}
}

void AChunkCharacter::TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation)
{
	if (!IsState(EEnemyStateName::HIT))
	{
		// Store hit direction
		FVector HitDirection = GetActorLocation() - InstigatorPawn->GetActorLocation();
		HitDirection = HitDirection.GetUnsafeNormal2D();
		EnemyBlackboard->SetValueAsVector("HitDirection", HitDirection);

		// Apply damage
		HealthComp->ChangeHealth(-1);

		// Hit Effect - Particles + Screen shake
		StartHitEffect();

		// Change state
		ChangeState(EEnemyStateName::HIT);
	}
}


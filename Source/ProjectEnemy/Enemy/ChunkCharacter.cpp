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

	// Rotation settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	// Movement settings
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 360.0f);
	GetCharacterMovement()->MaxAcceleration = 200.0f;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	// Health settings
	HealthComp->MaxHealth = 2;
}

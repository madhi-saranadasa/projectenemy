// Fill out your copyright notice in the Description page of Project Settings.


#include "BlobCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "EnemyHealthComponent.h"


ABlobCharacter::ABlobCharacter()
{
	// Capsule component settings
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);
	GetCapsuleComponent()->SetCapsuleRadius(25.0f);

	// Health settings
	HealthComp->MaxHealth = 2;
}


void ABlobCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// If we hit something during the attack, then stop the attack
	// Using dot product to check if the collision is in front of us or behind us
	// Stop attack if we hit something in front of us
	if (IsState(EEnemyStateName::ATTACK))
	{
		FVector DirectionVector = OtherActor->GetActorLocation() - GetActorLocation();
		DirectionVector.Normalize();
		float FrontJudgement = FVector::DotProduct(GetActorLocation(), DirectionVector);

		if (FrontJudgement >= 0)
		{
			ChangeState(EEnemyStateName::GRAZE);
		}
	}
}

void ABlobCharacter::SightResponse(ACharacter* InstigatorCharacter)
{
	if (IsState(EEnemyStateName::GRAZE))
	{
		EnemyBlackboard->SetValueAsObject("TargetCharacter", InstigatorCharacter);
		ChangeState(EEnemyStateName::ATTACK);
	}
}
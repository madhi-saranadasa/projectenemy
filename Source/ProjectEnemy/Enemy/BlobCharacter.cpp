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


void ABlobCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeBlackboard();
}


void ABlobCharacter::TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation)
{
	if (!IsState(EBlobStateName::HIT))
	{
		// Store hit direction
		FVector HitDirection = GetActorLocation() - InstigatorPawn->GetActorLocation();
		HitDirection = HitDirection.GetUnsafeNormal2D();
		BBComp->SetValueAsVector("HitDirection", HitDirection);

		// Play hit particles
		UNiagaraFunctionLibrary::SpawnSystemAttached(HitParticles, GetRootComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);

		// Apply damage
		HealthComp->ChangeHealth(-1);

		// Change state
		ChangeState(EBlobStateName::HIT);
	}
}


void ABlobCharacter::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}


void ABlobCharacter::OnSight(ACharacter* InstigatorCharacter)
{
	if (IsState(EBlobStateName::GRAZE))
	{
		BBComp->SetValueAsObject("TargetCharacter", InstigatorCharacter);
		ChangeState(EBlobStateName::ATTACK);
	}
}


void ABlobCharacter::ExecuteDeath()
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(HitParticles, GetRootComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);
	Destroy();
}


void ABlobCharacter::InitializeBlackboard()
{
	ChangeState(EBlobStateName::GRAZE);
	BBComp->SetValueAsVector("GrazeCenter", GetActorLocation());
}


bool ABlobCharacter::IsState(EBlobStateName InputState)
{
	return BBComp->GetValueAsEnum("CurrentBlobState") == (uint8)InputState;
}


void ABlobCharacter::ChangeState(EBlobStateName InputState)
{
	BBComp->SetValueAsEnum("CurrentBlobState", (uint8)InputState);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BlobCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>


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
}


void ABlobCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeBlackboard();
}


void ABlobCharacter::TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation)
{
	if (BBComp->GetValueAsEnum("CurrentBlobState") != (uint8)EBlobStateName::HIT)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blob took damage"));

		// Store hit direction
		FVector HitDirection = GetActorLocation() - InstigatorPawn->GetActorLocation();
		HitDirection = HitDirection.GetUnsafeNormal2D();
		BBComp->SetValueAsVector("HitDirection", HitDirection);

		// Change state
		BBComp->SetValueAsEnum("CurrentBlobState", (uint8)EBlobStateName::HIT);

		// Play hit particles
		UNiagaraFunctionLibrary::SpawnSystemAttached(HitParticles, GetRootComponent(), FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);
	}
}


void ABlobCharacter::OnPawnSeen(APawn* Pawn)
{
	if (Pawn)
	{
		if (BBComp->GetValueAsEnum("CurrentBlobState") == (uint8)EBlobStateName::GRAZE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Blob saw a pawn"));
			BBComp->SetValueAsObject("TargetPawn", Pawn);
			BBComp->SetValueAsEnum("CurrentBlobState", (uint8)EBlobStateName::ATTACK);
		}
	}
}


void ABlobCharacter::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}


void ABlobCharacter::InitializeBlackboard()
{
	BBComp->SetValueAsVector("GrazeCenter", GetActorLocation());
	BBComp->SetValueAsEnum("CurrentBlobState", (uint8)EBlobStateName::GRAZE);
}

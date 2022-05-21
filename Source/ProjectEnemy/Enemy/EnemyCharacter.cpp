// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Perception/PawnSensingComponent.h>
#include <Components/CapsuleComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>
#include "EnemyHealthComponent.h"
#include "EnemySightComponent.h"
#include "../Framework/PawnInterface.h"
#include "../Player/PlayerCharacter.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create health component
	HealthComp = CreateDefaultSubobject<UEnemyHealthComponent>(TEXT("HealthComp"));

	// Create sight component
	SightComp = CreateDefaultSubobject<UEnemySightComponent>(TEXT("SightComp"));

	// Rotation settings
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;

	// Default movement settings
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->MaxAcceleration = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = 125.0f;

	// RVO
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	
	// Collision Profile
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Disable any collision for the mesh
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
}


void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Hook up events
	HealthComp->HealthReachedZero.AddDynamic(this, &AEnemyCharacter::MarkForDeath);
	SightComp->SightResponse.AddDynamic(this, &AEnemyCharacter::SightResponse);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlap);

}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to blackboard for use during play
	RegisterBlackboardComponent();

	// Initialize blackboard with starting data
	InitializeBlackboard();
}


void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEnemyCharacter::TakeDamage_Implementation(AActor* InstigatorActor, FVector HitLocation, EDamageType IncomingDamageType)
{
	if (IncomingDamageType == EDamageType::PLAYER)
	{
		if (!IsState(EEnemyStateName::HIT))
		{
			// Store hit direction
			FVector HitDirection = GetActorLocation() - InstigatorActor->GetActorLocation();
			HitDirection = HitDirection.GetUnsafeNormal2D();
			EnemyBlackboard->SetValueAsVector("HitDirection", HitDirection);


			// Apply damage
			HealthComp->ChangeHealth(-1);

			// Screen Shake
			StartHitEffect();

			// Change state
			ChangeState(EEnemyStateName::HIT);
		}
	}
}


void AEnemyCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPawnInterface>())
	{
		IPawnInterface::Execute_TakeDamage(OtherActor, this, SweepResult.Location, EDamageType::ENEMYBODY);
	}
}

void AEnemyCharacter::SightResponse(ACharacter* InstigatorCharacter)
{

}


void AEnemyCharacter::MarkForDeath()
{
	bMarkedForDeath = true;
}


void AEnemyCharacter::ExecuteDeath()
{
	StartDeathEffect();
	Destroy();
}


bool AEnemyCharacter::GetDeathStatus()
{
	return bMarkedForDeath;
}


void AEnemyCharacter::RegisterBlackboardComponent()
{
	AAIController* EnemyController = Cast<AAIController>(GetController());

	if (EnemyController)
	{
		EnemyBlackboard = EnemyController->GetBlackboardComponent();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Getting enemy controller failed"));
	}
}


void AEnemyCharacter::InitializeBlackboard()
{
	ChangeState(EEnemyStateName::GRAZE);
	EnemyBlackboard->SetValueAsVector("GrazeCenter", GetActorLocation());
}


bool AEnemyCharacter::IsState(EEnemyStateName InputState)
{
	return EnemyBlackboard->GetValueAsEnum("CurrentEnemyState") == (uint8)InputState;
}


void AEnemyCharacter::ChangeState(EEnemyStateName InputState)
{
	EnemyBlackboard->SetValueAsEnum("CurrentEnemyState", (uint8)InputState);
}
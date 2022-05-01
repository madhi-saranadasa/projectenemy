// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Perception/PawnSensingComponent.h>
#include <Components/CapsuleComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>
#include "EnemyHealthComponent.h"
#include "EnemySightComponent.h"


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
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.0f;
	
	// Collision Profile
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// Disable camera collision on mesh
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}


void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Hook up events
	HealthComp->HealthReachedZero.AddDynamic(this, &AEnemyCharacter::MarkForDeath);
	SightComp->SightResponse.AddDynamic(this, &AEnemyCharacter::SightResponse);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnCharacterHit);

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


void AEnemyCharacter::TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation)
{

}


void AEnemyCharacter::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

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

}


bool AEnemyCharacter::GetDeathStatus()
{
	return bMarkedForDeath;
}
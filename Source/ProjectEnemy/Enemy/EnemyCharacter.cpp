// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
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
}


void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Add events
	HealthComp->OnDeath.AddDynamic(this, &AEnemyCharacter::MarkForDeath);
	SightComp->OnSight.AddDynamic(this, &AEnemyCharacter::OnSight);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnCharacterHit);

}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to blackboard for use during play
	RegisterBlackboardComponent();
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
		BBComp = EnemyController->GetBlackboardComponent();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Getting enemy controller failed"));
	}
}


void AEnemyCharacter::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}


void AEnemyCharacter::MarkForDeath()
{
	bMarkedForDeath = true;
}


void AEnemyCharacter::OnSight(ACharacter* InstigatorCharacter)
{

}


void AEnemyCharacter::ExecuteDeath()
{

}


bool AEnemyCharacter::GetDeathStatus()
{
	return bMarkedForDeath;
}


void AEnemyCharacter::TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation)
{

}

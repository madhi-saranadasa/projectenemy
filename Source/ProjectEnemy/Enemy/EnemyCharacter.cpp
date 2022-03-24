// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include <Perception/PawnSensingComponent.h>
#include <Components/CapsuleComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Pawn Sensing Component
	SenseComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SenseComp"));
}


void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Add events
	SenseComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnPawnSeen);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnCharacterHit);
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
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
}


void AEnemyCharacter::OnPawnSeen(APawn* Pawn)
{

}


void AEnemyCharacter::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}


void AEnemyCharacter::TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation)
{

}

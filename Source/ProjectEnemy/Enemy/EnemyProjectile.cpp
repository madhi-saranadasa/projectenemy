// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "NiagaraComponent.h"

AEnemyProjectile::AEnemyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create sphere component
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	// Projectile movement component
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->InitialSpeed = 0.0f;
	MoveComp->ProjectileGravityScale = 0.0f;

	// Niagara Component
	ProjectileVisual = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVisual"));
	ProjectileVisual->SetupAttachment(RootComponent);
	ProjectileVisual->bAutoActivate = true;

	// Collision Profile
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// No collision by default
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEnemyProjectile::ToggleCollisionAndSpeed(bool InputBool)
{
	if (InputBool)
	{
		MoveComp->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileSpeed);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		MoveComp->SetVelocityInLocalSpace(FVector::ZeroVector);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AEnemyProjectile::InitializeProjectile(AActor* Originator)
{
	if (Originator)
	{
		OriginatorActor = Originator;
		//SphereComp->IgnoreActorWhenMoving(Originator, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("For some reason, originator was null"));
	}
}


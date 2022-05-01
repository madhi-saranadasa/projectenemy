// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>


AEnemyProjectile::AEnemyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->InitialSpeed = 1000.0f;
	MoveComp->ProjectileGravityScale = 0.0f;
}


void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


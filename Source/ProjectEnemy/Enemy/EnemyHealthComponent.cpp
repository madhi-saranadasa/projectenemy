// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthComponent.h"

UEnemyHealthComponent::UEnemyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UEnemyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health to max health
	CurrentHealth = MaxHealth;
}


void UEnemyHealthComponent::ChangeHealth(int32 HealthDelta)
{
	// Apply the change
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthDelta, 0, MaxHealth);

	// Test if health has reached zero
	if (CurrentHealth == 0)
	{
		OnDeath.Broadcast();
	}
}

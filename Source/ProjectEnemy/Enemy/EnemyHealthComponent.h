// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTENEMY_API UEnemyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEnemyHealthComponent();

protected:

	virtual void BeginPlay() override;

public:
	/** Delegate to execute when we see a Pawn. */
	UPROPERTY(BlueprintAssignable)
	FEnemyDeathDelegate HealthReachedZero;

public:	

	void ChangeHealth(int32 HealthDelta);

	UPROPERTY(EditDefaultsOnly)
	int32 MaxHealth = 2;

private:
	
	int32 CurrentHealth;

};

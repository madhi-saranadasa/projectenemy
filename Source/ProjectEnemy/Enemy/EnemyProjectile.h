// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECTENEMY_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemyProjectile();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USphereComponent* SphereComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UProjectileMovementComponent* MoveComp;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};

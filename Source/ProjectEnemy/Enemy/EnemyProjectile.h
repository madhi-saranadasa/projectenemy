// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UNiagaraComponent* ProjectileVisual; 

	UPROPERTY(BlueprintReadOnly)
	AActor* OriginatorActor;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleCollisionAndSpeed(bool InputBool);

	UFUNCTION(BlueprintCallable)
	void InitializeProjectile(AActor* Originator); // Called by originator to set self-ignore

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ProjectileSpeed;
};

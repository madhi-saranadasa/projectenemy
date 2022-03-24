// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Framework/PawnInterface.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyCharacter.generated.h"

class UPawnSensingComponent;
class UNiagaraSystem;

UCLASS()
class PROJECTENEMY_API AEnemyCharacter : public ACharacter, public IPawnInterface
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

protected:

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* SenseComp;

	UPROPERTY()
	UBlackboardComponent* BBComp;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HitParticles;

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	

	virtual void Tick(float DeltaTime) override;

public:

	virtual void TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation);

	UFUNCTION()
	virtual void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	virtual void OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:

	void RegisterBlackboardComponent();


};

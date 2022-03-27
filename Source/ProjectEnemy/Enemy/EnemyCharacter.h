// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Framework/PawnInterface.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyCharacter.generated.h"

class UPawnSensingComponent;
class UEnemyHealthComponent;
class UEnemySightComponent;
class UNiagaraSystem;

UCLASS()
class PROJECTENEMY_API AEnemyCharacter : public ACharacter, public IPawnInterface
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

protected:

	UPROPERTY(EditDefaultsOnly)
	UEnemyHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly)
	UEnemySightComponent* SightComp;

	UPROPERTY()
	UBlackboardComponent* BBComp;

protected:

	bool bMarkedForDeath = false;

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	

	virtual void Tick(float DeltaTime) override;

public:

	virtual void TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation);

	UFUNCTION()
	virtual void OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void MarkForDeath();

	UFUNCTION()
	virtual void OnSight(ACharacter* InstigatorCharacter);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void ExecuteDeath();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual bool GetDeathStatus();

private:

	void RegisterBlackboardComponent();

};

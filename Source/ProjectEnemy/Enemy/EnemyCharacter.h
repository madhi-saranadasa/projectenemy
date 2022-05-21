// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Framework/PawnInterface.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyStateName : uint8 {
	GRAZE		UMETA(DisplayName = "Graze"),
	ATTACK		UMETA(DisplayName = "Attack"),
	HIT			UMETA(DisplayName = "Hit"),
	PURSUIT		UMETA(DisplayName = "Pursuit")
};

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

	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* EnemyBlackboard;

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	

	virtual void Tick(float DeltaTime) override;

protected:

	// Receive damage from others
	virtual void TakeDamage_Implementation(AActor* InstigatorActor, FVector HitLocation, EDamageType IncomingDamageType);

	// Trigger effects on overlapped actors
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// See player in vision cone
	UFUNCTION()
	virtual void SightResponse(ACharacter* InstigatorCharacter);
	
protected:

	// VARIABLES FOR DEATH

	bool bMarkedForDeath = false;

	// Return death status
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual bool GetDeathStatus();

	// Step 1 - Flip death bool to true
	UFUNCTION()
	virtual void MarkForDeath();

	// Step 2 - Implementation of death
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void ExecuteDeath();


public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void StartDeathEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void StartHitEffect();

protected:

	void RegisterBlackboardComponent();

	void InitializeBlackboard();

	bool IsState(EEnemyStateName InputState);

	void ChangeState(EEnemyStateName InputState);

};

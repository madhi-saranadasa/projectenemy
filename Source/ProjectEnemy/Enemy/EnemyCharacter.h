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

	bool bMarkedForDeath = false;

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void ApplyDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation);

	UFUNCTION()
	virtual void OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	virtual void SightResponse(ACharacter* InstigatorCharacter);
	
	UFUNCTION()
	virtual void MarkForDeath();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void ExecuteDeath();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual bool GetDeathStatus();

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

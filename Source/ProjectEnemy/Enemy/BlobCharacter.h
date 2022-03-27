// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "BlobCharacter.generated.h"

UENUM(BlueprintType)
enum class EBlobStateName : uint8 {
	GRAZE		UMETA(DisplayName = "Graze"),
	ATTACK		UMETA(DisplayName = "Attack"),
	HIT			UMETA(DisplayName = "Hit")
};


UCLASS()
class PROJECTENEMY_API ABlobCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public: 

	ABlobCharacter();

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* DeathParticles;

public:

	virtual void TakeDamage_Implementation(APawn* InstigatorPawn, FVector HitLocation) override;

	virtual void OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnSight(ACharacter* InstigatorCharacter);

	void ExecuteDeath();

private:

	void InitializeBlackboard();

	bool IsState(EBlobStateName InputState);

	void ChangeState(EBlobStateName InputState);
	
};

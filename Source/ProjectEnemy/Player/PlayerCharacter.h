// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerStateMachine.h"
#include "../Framework/PawnInterface.h"
#include "PlayerCharacter.generated.h"

class UPlayerStateMachine;
class UAttackVolume;
class UAnimMontage;
class UNiagaraComponent;
class UEnemyHealthComponent;

UCLASS()
class PROJECTENEMY_API APlayerCharacter : public ACharacter, public IPawnInterface
{
	GENERATED_BODY()

public:

	APlayerCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPlayerStateMachine* StateMachine;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttackVolume* AttackVolume;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UEnemyHealthComponent* HealthComp;

public:

	void StartMontage(UAnimMontage* InputAnim);

	virtual void TakeDamage_Implementation(AActor* InstigatorActor, FVector HitLocation, EDamageType IncomingDamageType);

	void OnAttackSuccess();

	void UpdateMoveCompParameters(float NewSpeed, float NewAccerlation, bool bIgnoreInput, bool bNewOrientToMovement);

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Particles")
	void ToggleChargeParticle(bool InputBool);

	UFUNCTION(BlueprintImplementableEvent, Category = "Particles")
	void PlayParticleAttack();

	UFUNCTION(BlueprintImplementableEvent, Category = "Particles")
	void PlaySecondaryAttack();

	UFUNCTION(BlueprintImplementableEvent, Category = "Particles")
	void PlayPrimaryFlash();

	UFUNCTION(BlueprintImplementableEvent, Category = "Particles")
	void PlaySecondaryFlash();

	UFUNCTION(BlueprintImplementableEvent, Category = "Particles")
	void PlayAttackParticles(bool bPrimary);

public:

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void OnDashPress();

	void OnAimStart();

	void OnAimEnd();

	FVector GetMousePosition();
};

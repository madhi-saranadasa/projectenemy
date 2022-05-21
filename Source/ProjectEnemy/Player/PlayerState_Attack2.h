// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState_Base.h"
#include "PlayerState_Attack2.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTENEMY_API UPlayerState_Attack2 : public UPlayerState_Base
{
	GENERATED_BODY()

public:

	UPlayerState_Attack2();

public:

	virtual void OnStateEnter() override;

	virtual void StateTick(float DeltaTime) override;

	virtual void OnStateExit() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float AttackSpeed = 1250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float AttackDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float AddedAttackCooldown = 0.4f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UCurveFloat* AttackCurve;

	FVector AttackVector;

private:

	UFUNCTION()
	void OnAttackEnd();

	UFUNCTION()
	void OnAttackCooldownEnd();

	FTimerHandle AttackTimerHandle;

	FTimerHandle AttackCooldownTimerHandle;
};

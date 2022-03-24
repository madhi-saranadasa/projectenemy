// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState_Base.h"
#include "PlayerState_Knockback.generated.h"

class UAnimMontage;

UCLASS()
class PROJECTENEMY_API UPlayerState_Knockback : public UPlayerState_Base
{
	GENERATED_BODY()

public:

	UPlayerState_Knockback();

public:

	virtual void OnStateEnter() override;

	virtual void StateTick(float DeltaTime) override;

	virtual void OnStateExit() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UAnimMontage* KnockbackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float KnockbackSpeed = 1250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float KnockbackDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float AddedKnockbackCooldown = 0.4f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UCurveFloat* KnockbackCurve;

private:

	UFUNCTION()
	void OnKnockbackEnd();

	UFUNCTION()
	void OnKnockbackCooldownEnd();

	FTimerHandle KnockbackTimerHandle;

	FTimerHandle KnockbackCooldownTimerHandle;
};

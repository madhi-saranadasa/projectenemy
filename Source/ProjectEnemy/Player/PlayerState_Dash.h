// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState_Base.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include "PlayerState_Dash.generated.h"

class UAnimMontage;

UCLASS(Blueprintable)
class PROJECTENEMY_API UPlayerState_Dash : public UPlayerState_Base
{
	GENERATED_BODY()

public:

	UPlayerState_Dash();

public:

	virtual void OnStateEnter() override;

	virtual void StateTick(float DeltaTime) override;

	virtual void OnStateExit() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UAnimMontage* DashAnim;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float DashSpeed = 1250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float DashDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float AddedDashCooldown = 0.4f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UCurveFloat* DashCurve;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	UNiagaraSystem* DashParticles;

	FVector DashVector;

private:

	UFUNCTION()
	void OnDashEnd();

	UFUNCTION()
	void OnDashCooldownEnd();

	FTimerHandle DashTimerHandle;

	FTimerHandle DashCooldownTimerHandle;

};

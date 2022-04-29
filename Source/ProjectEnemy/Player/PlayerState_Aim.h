// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState_Base.h"
#include "PlayerState_Aim.generated.h"


UCLASS()
class PROJECTENEMY_API UPlayerState_Aim : public UPlayerState_Base
{
	GENERATED_BODY()

public:

	UPlayerState_Aim();

public:

	virtual void OnStateEnter() override;

	virtual void StateTick(float DeltaTime) override;

	virtual void OnStateExit() override;

private:


	UPROPERTY(EditDefaultsOnly, Category = "State")
	float PrimaryDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float SecondaryDuration = 2.0f;

private:

	FTimerHandle PrimaryTimerHandle;

	FTimerHandle SecondaryTimerHandle;

	UFUNCTION()
	void PrimaryChargeReady();

	UFUNCTION()
	void SecondaryChargeReady();
};

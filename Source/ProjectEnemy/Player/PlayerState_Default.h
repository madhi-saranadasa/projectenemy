// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerState_Base.h"
#include "PlayerState_Default.generated.h"


UCLASS(Blueprintable)
class PROJECTENEMY_API UPlayerState_Default : public UPlayerState_Base
{
	GENERATED_BODY()

public:

	UPlayerState_Default();

public:

	virtual void OnStateEnter() override;

	virtual void StateTick(float DeltaTime) override;

	virtual void OnStateExit() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerCharacter.h"
#include "PlayerStateMachine.h"
#include "PlayerState_Base.generated.h"

class APlayerCharacter;

UCLASS(Blueprintable)
class PROJECTENEMY_API UPlayerState_Base : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "State")
	EPlayerStateName StateName;

protected:

	UPROPERTY()
	APlayerCharacter* OwningCharacter;

	UPROPERTY()
	UPlayerStateMachine* StateMachine;

public:

	virtual void OnStateEnter();

	virtual void StateTick(float DeltaTime);

	virtual void OnStateExit();

	void FillStateContext(APlayerCharacter* InputPC, UPlayerStateMachine* InputStateMachine);

};

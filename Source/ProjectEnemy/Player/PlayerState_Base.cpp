// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Base.h"

void UPlayerState_Base::OnStateEnter()
{

}


void UPlayerState_Base::StateTick(float DeltaTime)
{

}


void UPlayerState_Base::OnStateExit()
{

}

void UPlayerState_Base::FillStateContext(APlayerCharacter* InputPC, UPlayerStateMachine* InputStateMachine)
{
	OwningCharacter = InputPC;
	StateMachine = InputStateMachine;
}

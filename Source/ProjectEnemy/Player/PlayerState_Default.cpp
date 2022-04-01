// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Default.h"


UPlayerState_Default::UPlayerState_Default()
{
	StateName = EPlayerStateName::DEFAULT;
}


void UPlayerState_Default::OnStateEnter()
{

}


void UPlayerState_Default::StateTick(float DeltaTime)
{

}


void UPlayerState_Default::OnStateExit()
{
	Super::OnStateExit();
}

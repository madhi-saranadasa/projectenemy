// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Default.h"


UPlayerState_Default::UPlayerState_Default()
{
	StateName = EPlayerStateName::DEFAULT;
}


void UPlayerState_Default::OnStateEnter()
{
	// Set new control settings
	OwningCharacter->UpdateMoveCompParameters(Speed, Acceleration, false, true);

	UE_LOG(LogTemp, Warning, TEXT("Default"));
}


void UPlayerState_Default::StateTick(float DeltaTime)
{

}


void UPlayerState_Default::OnStateExit()
{
	Super::OnStateExit();
}

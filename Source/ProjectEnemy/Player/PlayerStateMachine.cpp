// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateMachine.h"
#include "PlayerState_Base.h"
#include "PlayerCharacter.h"
#include <NiagaraComponent.h>

UPlayerStateMachine::UPlayerStateMachine()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerStateMachine::BeginPlay()
{
	Super::BeginPlay();

	// Store reference to owner
	SetPlayerCharacter();

	// Instantiate states
	InstantiateState(DefaultState);
	InstantiateState(DashState);
	InstantiateState(KnockbackState);
	InstantiateState(AimState);
	InstantiateState(AttackState);
}


void UPlayerStateMachine::InstantiateState(TSubclassOf<UPlayerState_Base> InputState)
{
	// check if blueprint is assigned
	if (!InputState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to instantiate class but nothing is assigned"));
		return;
	}

	// Instantiate add to the list of available states
	UPlayerState_Base* NewState = NewObject<UPlayerState_Base>(this, InputState);
	PlayerStates.Add(NewState);

	// Fill context for state to use
	NewState->FillStateContext(OwningPC, this);
}


void UPlayerStateMachine::StateTick(float DeltaTime)
{
	// Call tick from the current state
	CurrentState->StateTick(DeltaTime);
}


void UPlayerStateMachine::ChangeState(EPlayerStateName InputStateName)
{
	// Store what the next and previous states
	if (CurrentState)
	{
		PreviousState = GetCurrentState();
	}
	
	NextState = InputStateName;

	// Exit current state
	if (CurrentState)
	{
		CurrentState->OnStateExit();
	}

	// Loop through instantiated state and find the one that matches the input enum
	// Set to the current state and enter state
	for (UPlayerState_Base* State : PlayerStates)
	{
		if (State && State->StateName == InputStateName)
		{
			CurrentState = State;
			CurrentState->OnStateEnter();
		}
	}
}


EPlayerStateName UPlayerStateMachine::GetCurrentState()
{
	// Other classes can ask what the current state is
	return CurrentState->StateName;
}


void UPlayerStateMachine::SetAiming(bool InputBool)
{
	bAiming = InputBool;
}


void UPlayerStateMachine::SetJustCharged(bool InputBool)
{
	bJustCharged = InputBool;
}


void UPlayerStateMachine::SetPlayerCharacter()
{
	// Store a player reference for later use. Not really used yet
	APlayerCharacter* PCCast = Cast<APlayerCharacter>(GetOwner());

	if (PCCast)
	{
		OwningPC = PCCast;
	}
}
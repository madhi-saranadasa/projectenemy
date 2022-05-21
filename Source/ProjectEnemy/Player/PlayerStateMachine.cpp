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

	// Instantiate states
	InstantiateState(DefaultState);
	InstantiateState(DashState);
	InstantiateState(KnockbackState);
	InstantiateState(AimState);
	InstantiateState(AttackState);
	InstantiateState(Attack2State);
}


void UPlayerStateMachine::InstantiateState(TSubclassOf<UPlayerState_Base> InputState)
{
	// Check if state asset is assigned in details panel
	if (!InputState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to instantiate class but nothing is assigned"));
		return;
	}


	// Prepare reference of player character for states to hold
	APlayerCharacter* PlayerCharacterCast = Cast<APlayerCharacter>(GetOwner());

	if (!PlayerCharacterCast)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to store player character reference on state machine"));
		return;
	}

	// Instantiate add to the list of available states
	UPlayerState_Base* NewState = NewObject<UPlayerState_Base>(this, InputState);
	PlayerStates.Add(NewState);

	// Fill context for state to use
	NewState->FillStateContext(PlayerCharacterCast, this);
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
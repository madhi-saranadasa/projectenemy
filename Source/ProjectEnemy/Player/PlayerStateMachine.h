// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateMachine.generated.h"

class UPlayerState_Base;
class APlayerCharacter;


UENUM(BlueprintType)
enum class EPlayerStateName : uint8 {
	DEFAULT			UMETA(DisplayName = "Default"),
	DASH			UMETA(DisplayName = "Dash"),
	KNOCKBACK		UMETA(DisplayName = "Knockback"),
	AIM				UMETA(DisplayName = "Aim"),
	ATTACK			UMETA(DisplayName = "Attack"),
	ATTACK2			UMETA(DisplayName = "Attack2")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTENEMY_API UPlayerStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateMachine();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	APlayerCharacter* OwningPlayerCharacter;

	UPROPERTY()
	EPlayerStateName CurrentStateName;

	UPROPERTY()
	UPlayerState_Base* CurrentState;

	UPROPERTY()
	TArray<UPlayerState_Base*> PlayerStates;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TSubclassOf<UPlayerState_Base> DefaultState;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TSubclassOf<UPlayerState_Base> DashState;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TSubclassOf<UPlayerState_Base> KnockbackState;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TSubclassOf<UPlayerState_Base> AimState;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TSubclassOf<UPlayerState_Base> AttackState;

	UPROPERTY(EditDefaultsOnly, Category = "States")
	TSubclassOf<UPlayerState_Base> Attack2State;

private:

	void InstantiateState(TSubclassOf<UPlayerState_Base> InputState);

public:

	void ChangeState(EPlayerStateName InputStateName);

	void StateTick(float DeltaTime);
	
	EPlayerStateName GetCurrentState();

public: 

	// These variables are the blackboard holding all the state information

	EPlayerStateName PreviousState; // set during ChangeState

	EPlayerStateName NextState; // set during ChangeState, only safe to access during OnStateEnter

	FVector KnockbackVector;

	bool bCanDash; // Monitors dash cooldown

	bool bCanBeHit; // Monitors hit cooldown to prevent stun locking

	bool bCanAttack; // Monitors attack cooldown

	bool bAttackLanded;

	bool bAiming;

	bool bJustCharged;

	bool bChargePrimary;

	bool bChargeSecondary;

};

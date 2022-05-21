// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerStateMachine.h"
#include "PlayerState_Base.h"
#include <Animation/AnimMontage.h>
#include "../Enemy/EnemyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <DrawDebugHelpers.h>
#include "AttackVolume.h"
#include <NiagaraComponent.h>
#include "../Enemy/EnemyHealthComponent.h"


#pragma region HOUSEKEEPING

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule component settings
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);
	GetCapsuleComponent()->SetCapsuleRadius(25.0f);

	// Create state machine component
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));

	// Create attack volume component
	AttackVolume = CreateDefaultSubobject<UAttackVolume>(TEXT("AttackVolume"));

	// Create health component
	HealthComp = CreateDefaultSubobject<UEnemyHealthComponent>(TEXT("HealthComp"));

	// Initialize Rotation settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	// Set up collision profile
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Inputs - only default state permits user input
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::OnDashPress);
	PlayerInputComponent->BindAction("AimToggle", IE_Pressed, this, &APlayerCharacter::OnAimStart);
	PlayerInputComponent->BindAction("AimToggle", IE_Released, this, &APlayerCharacter::OnAimEnd);
}


void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// TODO: move this to state machine and iterate through instantiated states
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

#pragma endregion


#pragma region INPUT HANDLING

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(FVector::ForwardVector, AxisValue);
}


void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FVector::RightVector, AxisValue);
}


void APlayerCharacter::OnDashPress()
{
	// Go to dash state if permitted
	if (StateMachine->bCanDash)
	{
		if (StateMachine->GetCurrentState() == EPlayerStateName::DEFAULT)
		{
			StateMachine->ChangeState(EPlayerStateName::DASH);
		}
		else if (StateMachine->GetCurrentState() == EPlayerStateName::AIM)
		{
			StateMachine->ChangeState(EPlayerStateName::DASH);
		}
	}
}


void APlayerCharacter::OnAimStart()
{
	// Log aiming
	StateMachine->bAiming = true;

	// Go to aim state if permitted
	if (StateMachine->GetCurrentState() == EPlayerStateName::DEFAULT)
	{
		StateMachine->ChangeState(EPlayerStateName::AIM);
	}
}


void APlayerCharacter::OnAimEnd()
{
	// Log aiming
	StateMachine->bAiming = false;

	// Exit aim state if currently in aim state
	if (StateMachine->GetCurrentState() == EPlayerStateName::AIM)
	{

		if (StateMachine->bChargeSecondary)
		{
			StateMachine->ChangeState(EPlayerStateName::ATTACK2);
		}
		else if (StateMachine->bChargePrimary)
		{
			StateMachine->ChangeState(EPlayerStateName::ATTACK);
		}
		else
		{
			StateMachine->ChangeState(EPlayerStateName::DEFAULT);
		}

		/*
		if (StateMachine->bChargePrimary | StateMachine->bChargeSecondary)
		{
			StateMachine->ChangeState(EPlayerStateName::ATTACK);
		}
		else
		{
			StateMachine->ChangeState(EPlayerStateName::DEFAULT);
		}

		*/

	}
}


FVector APlayerCharacter::GetMousePosition()
{
	// States can use this to get the mouse position (for aiming)

	// Get player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Fill mouse variables
	FVector MousePosition;
	FVector MouseDirection;
	PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection);

	// Raycast information
	FVector RayOrigin = PlayerController->PlayerCameraManager->GetCameraLocation();
	FVector RayDirection = MousePosition - RayOrigin;

	// Find point on plane through  raycast
	FVector OutputVector = FMath::RayPlaneIntersection(RayOrigin, RayDirection, FPlane(FVector::ZeroVector, FVector::UpVector));

	return OutputVector;
}

#pragma  endregion


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initializing the state machine
	StateMachine->ChangeState(EPlayerStateName::DEFAULT);
	StateMachine->bCanDash = true;
	StateMachine->bCanBeHit = true;
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Just update the state machine
	StateMachine->StateTick(DeltaTime);
}


void APlayerCharacter::StartMontage(UAnimMontage* InputAnim)
{
	float MontageOutput = PlayAnimMontage(InputAnim, 1.0f, FName("Default"));
}


void APlayerCharacter::TakeDamage_Implementation(AActor* InstigatorActor, FVector HitLocation, EDamageType IncomingDamageType)
{
	if (!StateMachine->bCanBeHit)
	{
		// exit out if hit is on cooldown
		return;
	}

	EPlayerStateName CurrentStateCompare = StateMachine->GetCurrentState();


	// Enemy bodies can only hurt in default and aim
	if (IncomingDamageType == EDamageType::ENEMYBODY)
	{
		if (CurrentStateCompare == EPlayerStateName::DEFAULT || CurrentStateCompare == EPlayerStateName::AIM)
		{
			// Calculate the knockback vector and store in the state machine blackboard

			FVector NewVector = GetActorLocation() - InstigatorActor->GetActorLocation();
			NewVector = NewVector.GetUnsafeNormal2D();

			StateMachine->KnockbackVector = NewVector;
			StateMachine->ChangeState(EPlayerStateName::KNOCKBACK);

			return;
		}
	}

	// Enemy projectiles can hurt in default, aim, and normal attack
	if (IncomingDamageType == EDamageType::ENEMYPROJ)
	{
		if (CurrentStateCompare == EPlayerStateName::DEFAULT || CurrentStateCompare == EPlayerStateName::AIM || CurrentStateCompare == EPlayerStateName::ATTACK)
		{
			// Calculate the knockback vector and store in the state machine blackboard

			FVector NewVector = GetActorLocation() - InstigatorActor->GetActorLocation();
			NewVector = NewVector.GetUnsafeNormal2D();

			StateMachine->KnockbackVector = NewVector;
			StateMachine->ChangeState(EPlayerStateName::KNOCKBACK);

			return;
		}
	}
}


void APlayerCharacter::OnAttackSuccess()
{
	StateMachine->bAttackLanded = true;
}


void APlayerCharacter::UpdateMoveCompParameters(float NewSpeed, float NewAccerlation, bool bIgnoreInput, bool bNewOrientToMovement)
{
	// States can use this to establish movement
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	GetCharacterMovement()->MaxAcceleration = NewAccerlation;
	GetCharacterMovement()->bOrientRotationToMovement = bNewOrientToMovement;
	GetController()->SetIgnoreMoveInput(bIgnoreInput);
}
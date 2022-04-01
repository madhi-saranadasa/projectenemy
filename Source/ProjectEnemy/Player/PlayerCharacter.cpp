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

	// Rotation settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	// Movement settings
	GetCharacterMovement()->MaxAcceleration = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Inputs - only default state permits user input
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Shift", IE_Pressed, this, &APlayerCharacter::OnShiftPress);
	PlayerInputComponent->BindAction("AimToggle", IE_Pressed, this, &APlayerCharacter::OnAimStart);
	PlayerInputComponent->BindAction("AimToggle", IE_Released, this, &APlayerCharacter::OnAimEnd);
}


void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Add events
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnCharacterHit);
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initializing the state machine
	StateMachine->ChangeState(EPlayerStateName::DEFAULT);
	StateMachine->SetCanBeHit(true);
	StateMachine->SetCanDash(true);
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Just update the state machine
	StateMachine->StateTick(DeltaTime);
}


void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// TODO: move this to state machine and iterate through instantiated states
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(FVector::ForwardVector, AxisValue);
}


void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FVector::RightVector, AxisValue);
}


void APlayerCharacter::StartMontage(UAnimMontage* InputAnim)
{
	// Play montages on mesh, should only called by the state machine
	float MontageOutput = PlayAnimMontage(InputAnim, 1.0f, FName("Default"));
}


void APlayerCharacter::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
	// Try progressing to knockback state if hit by AEnemyCharacter
	AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OtherActor);

	if (HitEnemy)
	{
		if (StateMachine->GetCurrentState() == EPlayerStateName::DEFAULT && StateMachine->bCanBeHit)
		{
			// Calculate the knockback vector and store in the state machine blackboard

			FVector NewVector = GetActorLocation() - OtherActor->GetActorLocation();
			NewVector = NewVector.GetUnsafeNormal2D();

			StateMachine->KnockbackVector = NewVector;
			StateMachine->ChangeState(EPlayerStateName::KNOCKBACK);

		}
	}
}


void APlayerCharacter::OnAttackSuccess()
{
	StateMachine->bAttackLanded = false;
}


void APlayerCharacter::OnShiftPress()
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
	StateMachine->SetAiming(true);

	// Go to aim state if permitted
	if (StateMachine->GetCurrentState() == EPlayerStateName::DEFAULT)
	{
		StateMachine->ChangeState(EPlayerStateName::AIM);
	}
}


void APlayerCharacter::OnAimEnd()
{
	// Log aiming
	StateMachine->SetAiming(false);

	// Exit aim state if currently in aim state
	if (StateMachine->GetCurrentState() == EPlayerStateName::AIM)
	{
		if (StateMachine->bChargeReady)
		{
			StateMachine->ChangeState(EPlayerStateName::ATTACK);
		}
		else
		{
			StateMachine->ChangeState(EPlayerStateName::DEFAULT);
		}
		
	}
}


void APlayerCharacter::UpdateMoveCompParameters(float NewSpeed, float NewAccerlation, bool bNewOrientToMovement)
{
	// States can use this to modify the default movement mode
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	GetCharacterMovement()->MaxAcceleration = NewAccerlation;
	GetCharacterMovement()->bOrientRotationToMovement = bNewOrientToMovement;
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
	//DrawDebugBox(GetWorld(), OutputVector, FVector::OneVector * 50.0f, FQuat::Identity, FColor::Red, false, 1.0f, 0, 2.0f);

	return OutputVector;

}

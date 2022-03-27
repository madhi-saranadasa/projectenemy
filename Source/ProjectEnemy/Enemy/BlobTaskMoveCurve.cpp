// Fill out your copyright notice in the Description page of Project Settings.


#include "BlobTaskMoveCurve.h"
#include <AIController.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>


uint16 UBlobTaskMoveCurve::GetInstanceMemorySize() const
{
	return sizeof(FMoveCurveData);
}


EBTNodeResult::Type UBlobTaskMoveCurve::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Initialize node memory
	FMoveCurveData* MyMemory = reinterpret_cast<FMoveCurveData*>(NodeMemory);
	
	// Fill data
	MyMemory->TimeElapsed = 0.0f;

	// Play animation
	OwnerComp.GetAIOwner()->GetCharacter()->PlayAnimMontage(MoveAnim, 1.0f, FName("Default"));

	// Pass on to tick
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}


void UBlobTaskMoveCurve::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Access node memory
	FMoveCurveData* MyMemory = reinterpret_cast<FMoveCurveData*>(NodeMemory);

	// Increment time on curve
	MyMemory->TimeElapsed += DeltaSeconds;

	// Move
	float AlphaPoint = MoveCurve->GetFloatValue(MyMemory->TimeElapsed/ MoveDuration);
	FVector MoveDirection = OwnerComp.GetBlackboardComponent()->GetValueAsVector(MoveDirectionKey.SelectedKeyName);
	OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->MoveSmooth(MoveDirection * MaxMoveSpeed * AlphaPoint, DeltaSeconds);

	// Finish task if time is reached
	if (MyMemory->TimeElapsed >= MoveDuration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

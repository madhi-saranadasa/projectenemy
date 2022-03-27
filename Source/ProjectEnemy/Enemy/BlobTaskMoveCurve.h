// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BlobTaskMoveCurve.generated.h"

// see All about BTTasks in C++ by Kaos Spectrum at https://www.thegames.dev/?p=70

class UAnimMontage;

struct FMoveCurveData
{
	float TimeElapsed = 0.0f;
};

UCLASS()
class PROJECTENEMY_API UBlobTaskMoveCurve : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float MoveDuration;

	UPROPERTY(EditAnywhere)
	float MaxMoveSpeed;

	UPROPERTY(EditAnywhere)
	UCurveFloat* MoveCurve;

	UPROPERTY(EditAnywhere)
	UAnimMontage* MoveAnim;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MoveDirectionKey;

public:

	virtual uint16 GetInstanceMemorySize() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

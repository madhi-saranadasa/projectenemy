// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPawnInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTENEMY_API IPawnInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeDamage(AActor* InstigatorActor, FVector HitLocation, bool bSourceIsEnemy);
};

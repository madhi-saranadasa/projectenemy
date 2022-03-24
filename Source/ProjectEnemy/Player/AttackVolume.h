// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackVolume.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTENEMY_API UAttackVolume : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttackVolume();

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FVector AttackOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackRadius;

public:

	void SweepAttackVolume(float DebugDrawDuration);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hazard.generated.h"

UCLASS()
class PROJECTENEMY_API AHazard : public AActor
{
	GENERATED_BODY()
	
public:	

	AHazard();

protected:



protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};

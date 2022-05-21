// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "ChunkCharacter.generated.h"


UCLASS()
class PROJECTENEMY_API AChunkCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:

	AChunkCharacter();

public:

	virtual void SightResponse(ACharacter* InstigatorCharacter);
};

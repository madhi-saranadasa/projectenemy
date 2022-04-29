// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "ChunkCharacter.generated.h"


UENUM(BlueprintType)
enum class EChunkStateName : uint8 {
	GRAZE		UMETA(DisplayName = "Graze"),
	CHASE		UMETA(DisplayName = "Chase"),
	ATTACK		UMETA(DisplayName = "Attack"),
	HIT			UMETA(DisplayName = "Hit")
};

UCLASS()
class PROJECTENEMY_API AChunkCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:

	AChunkCharacter();
	
};

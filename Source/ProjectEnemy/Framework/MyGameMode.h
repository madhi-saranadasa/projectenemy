// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MyGameMode.generated.h"

class UEnvQuery;
class AEnemyCharacter;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class PROJECTENEMY_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMyGameMode();

public:

	virtual void StartPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float SpawnCheckInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	UEnvQuery* SpawnQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	TSubclassOf<AEnemyCharacter> EnemyAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	bool bActiveSpawing;

	FTimerHandle TimerHandle_SpawnCheck;

protected:

	void PerformSpawnCheck();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};

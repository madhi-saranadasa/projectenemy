// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "../Enemy/EnemyCharacter.h"


AMyGameMode::AMyGameMode()
{
	SpawnCheckInterval = 2.0f;
	bActiveSpawing = false;
}


void AMyGameMode::StartPlay()
{
	Super::StartPlay();

	// Set up timer to perform the spawn check
	if (bActiveSpawing)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnCheck, this, &AMyGameMode::PerformSpawnCheck, SpawnCheckInterval, true);
	}
}


void AMyGameMode::PerformSpawnCheck()
{
	// Run the query instance async 
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	// Hook things up to run when everything is completed
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameMode::OnQueryCompleted);
	}
}


void AMyGameMode::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something failed with the EQS Query"));
		return;
	}

	TArray<FVector> Location = QueryInstance->GetResultsAsLocations();

	if (Location.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(EnemyAsset, Location[0], FRotator::ZeroRotator);
	}
}

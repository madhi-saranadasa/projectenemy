// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "../Enemy/EnemyCharacter.h"
#include <EngineUtils.h>


AMyGameMode::AMyGameMode()
{
	SpawnCheckInterval = 4.0f;
	bActiveSpawing = false;
}


void AMyGameMode::StartPlay()
{
	Super::StartPlay();

	// Set up timer to perform the spawn check
	if (bActiveSpawing)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnCheck, this, &AMyGameMode::SpawnRequest, SpawnCheckInterval, true);
	}
}


void AMyGameMode::SpawnRequest()
{
	// check if there are no enemies, then spawn more if needed
	int32 NumEnemies = 0;

	for (TActorIterator<AEnemyCharacter> It(GetWorld()); It; ++It)
	{
		NumEnemies++;
	}

	if (NumEnemies > 0)
	{
		return;
	}


	// Grab a random wave from the pre-specified list
	int32 WaveID = FMath::RandRange(0, WaveList.Num() - 1);

	// Check if wave list data is assigned on the blueprint
	if (WaveList.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No wave list data, aborting"));
		return;
	}

	EnemyListForWave.Init(0, WaveList[WaveID].X); // Clear out and create blob entries
	ChunkList.Init(1, WaveList[WaveID].Y); // Create chunk entries
	EnemyListForWave.Append(ChunkList); // Appending everything

	// Check if something failed with the final list
	if (EnemyListForWave.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy list has no elements, aborting"));
		return;
	}

	// Run the query instance async 
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

	// Hook things up to run when everything is completed
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameMode::RenderSpawnRequest);
	}

}


void AMyGameMode::RenderSpawnRequest(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// Check is EQS failed
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something failed with the EQS Query"));
		return;
	}

	TArray<FVector> EQSLocations = QueryInstance->GetResultsAsLocations();

	// Check if EQS list is at least as long as the number of enemies
	if (EnemyListForWave.Num() > EQSLocations.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough locations, aborting"));
		return;
	}

	for (int32 i = 0; i < EnemyListForWave.Num(); i++)
	{
		if (EnemyListForWave[i] == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Blob"));
			GetWorld()->SpawnActor<AActor>(BlobAsset, EQSLocations[FMath::RandRange(0, EQSLocations.Num() - 1)], FRotator::ZeroRotator);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Chunk"));
			GetWorld()->SpawnActor<AActor>(ChunkAsset, EQSLocations[FMath::RandRange(0, EQSLocations.Num() - 1)], FRotator::ZeroRotator);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DisplayManager.generated.h"

class AMyCamera;
class AMyPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTENEMY_API UDisplayManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UDisplayManager();

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset References")
	TSubclassOf<AMyCamera> CameraPawnAsset;

	UPROPERTY()
	AMyCamera* MyCamera;

	UPROPERTY()
	AMyPlayerController* OwningPC;

public:

	// Cache reference to owning player controller at start
	void RegisterPlayerController();

	// Spawns camera and sets as view target for the player controller
	void SpawnCamera();

	void ResetView();

		
};

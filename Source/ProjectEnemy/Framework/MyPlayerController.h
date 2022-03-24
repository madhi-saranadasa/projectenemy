// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UDisplayManager;
class APlayerCharacter;

UCLASS()
class PROJECTENEMY_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMyPlayerController();

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Components")
	UDisplayManager* DisplayManager;

	UPROPERTY(EditAnywhere, Category = "Asset References")
	TSubclassOf<APlayerCharacter> PlayerCharacterAsset;

	UPROPERTY()
	APlayerCharacter* CurrentPlayerCharacter;

protected:

	void OnSpaceBarPress();

	void SpawnPlayerOnGround();

};

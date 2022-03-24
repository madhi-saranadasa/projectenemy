// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCamera.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTENEMY_API AMyCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	// Starting values for camera position, needed for the Default Object

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FVector StartLocation = FVector(0.0f, 0.0f, 1000.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FRotator StartRotation = FRotator(-90.0f, 0.0f, 0.0f);

};

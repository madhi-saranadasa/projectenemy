// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Kismet/GameplayStatics.h>

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// Start behavior tree
	UE_LOG(LogTemp, Warning, TEXT("Starting the behavior tree"));
	RunBehaviorTree(BehaviorTree);
}

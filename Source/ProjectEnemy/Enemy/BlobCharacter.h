// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "BlobCharacter.generated.h"


UCLASS()
class PROJECTENEMY_API ABlobCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public: 

	ABlobCharacter();

public:

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void SightResponse(ACharacter* InstigatorCharacter);
	
};

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

	virtual void OnCharacterOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void SightResponse(ACharacter* InstigatorCharacter);
	
};

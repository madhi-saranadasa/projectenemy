// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemySightComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSightDelegate, ACharacter*, InstigatorCharacter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTENEMY_API UEnemySightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEnemySightComponent();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FSightDelegate SightResponse;

	FTimerHandle SightIntervalHandle;

public:

	UPROPERTY(EditAnywhere)
	float SightInterval;

	UPROPERTY(EditAnywhere)
	float SightRadius;

	UPROPERTY(EditAnywhere)
	FVector SightOffset;

private:

	UFUNCTION()
	void SightCheck();

	void SphereTraceMethod();

};

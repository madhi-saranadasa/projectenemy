// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackVolume.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Framework/PawnInterface.h"
#include "PlayerCharacter.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <DrawDebugHelpers.h>

UAttackVolume::UAttackVolume()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAttackVolume::BeginPlay()
{
	Super::BeginPlay();
}


void UAttackVolume::SweepAttackVolume(float DebugDrawDuration)
{
	// Find start and end
	FVector AttackStart = GetOwner()->GetActorLocation();
	FVector AttackEnd = AttackStart + GetOwner()->GetActorForwardVector() * AttackOffset.X + GetOwner()->GetActorRightVector() * AttackOffset.Y;

	// Array to store results
	TArray<FHitResult> HitArray;

	// Trace parameters
	TArray<TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	// Do the trace
	bool bAnyHits = UKismetSystemLibrary::SphereTraceMultiForObjects(this, AttackStart, AttackEnd, 40.0f,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Red, DebugDrawDuration);

	if (bAnyHits)
	{
		for (FHitResult Hit : HitArray)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				if (HitActor->Implements<UPawnInterface>())
				{
					IPawnInterface::Execute_ApplyDamage(HitActor, Cast<APawn>(GetOwner()), Hit.Location);
					
					APlayerCharacter* PCCast = Cast<APlayerCharacter>(GetOwner());
					if (PCCast)
					{
						PCCast->OnAttackSuccess();
					}
				}
			}
		}
	}
}


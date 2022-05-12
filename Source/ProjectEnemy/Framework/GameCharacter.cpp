// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"
#include <Components/CapsuleComponent.h>

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision Profile
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}


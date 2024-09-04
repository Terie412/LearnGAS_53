// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"


AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	bWantsPlayerState = true;
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


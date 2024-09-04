// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MyAIController.generated.h"

UCLASS()
class LEARNGAS_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

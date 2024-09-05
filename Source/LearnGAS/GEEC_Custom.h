// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_Custom.generated.h"

/**
 * 
 */
UCLASS()
class LEARNGAS_API UGEEC_Custom : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SkillTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<int> Indexes;

	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute TargetAttribute;
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
};

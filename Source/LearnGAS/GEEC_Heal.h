// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_Heal.generated.h"

/**
 * GE 必须是 Infinite Periodic 的 GE
 * 效果是先尝试瞬间恢复 PrimaryAttribute，如果有剩余恢复量，就缓慢加到 SecondaryAttribute 上 
 */
UCLASS()
class LEARNGAS_API UGEEC_Heal : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	/** 总计恢复的量 */
	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	/** 每次执行恢复的量 */
	UPROPERTY(EditDefaultsOnly)
	float HealEachExecution;

	/** 优先恢复的血量 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute PrimaryAttribute;

	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute MaxPrimaryAttribute;
	
	/** 当 PrimaryAttribute 满了之后，将溢出的恢复量加到 SecondaryAttribute 上 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute SecondaryAttribute;

	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute MaxSecondaryAttribute;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:

	/** 当前还剩多少恢复量 */
	mutable float HealLeft = -1;
};

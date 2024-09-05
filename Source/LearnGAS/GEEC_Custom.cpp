// Fill out your copyright notice in the Description page of Project Settings.


#include "GEEC_Custom.h"

#include "AbilitySystemComponent.h"
#include "MyAttributeSet.h"

void UGEEC_Custom::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	float TotalDamage = 0;
	for(int Index : Indexes)
	{
		TotalDamage += Index;
	}
	TotalDamage *= Damage;

	bool bFound = false;
	float AttributeValue = TargetASC->GetGameplayAttributeValue(TargetAttribute, bFound);
	if(bFound)
	{
		UE_LOG(LogTemp, Display, TEXT("qtc 扣除伤害 %f"), TotalDamage);
		TargetASC->SetNumericAttributeBase(TargetAttribute, AttributeValue - TotalDamage);
	}
}

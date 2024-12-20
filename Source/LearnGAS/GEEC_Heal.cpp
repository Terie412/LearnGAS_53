// Fill out your copyright notice in the Description page of Project Settings.


#include "GEEC_Heal.h"

#include "AbilitySystemComponent.h"

void UGEEC_Heal::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	FGameplayEffectSpec GESpec = ExecutionParams.GetOwningSpec();
	UE_LOG(LogTemp, Display, TEXT("qtc, UGEEC_Heal::Execute_Implementation DeltaTime = %f, ExecutionTimes = %d, TryExecutionTimes = %d, LastExecutionTime = %f, LastTryExecutionTime = %f"), ExecutionParams.DeltaTime, GESpec.ExecutionTimes, GESpec.TryExecutionTimes, GESpec.LastExecutionTime, GESpec.LastTryExecutionTime);
	TObjectPtr<const UGameplayEffect> CDO_GE = GESpec.Def;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	check(SourceASC);

	if (!TargetASC)
	{
		UE_LOG(LogTemp, Error, TEXT("UGEEC_Heal: TargetASC is invalid."));
		return;
	}

	if (!TargetASC->HasAttributeSetForAttribute(PrimaryAttribute) || !TargetASC->HasAttributeSetForAttribute(SecondaryAttribute))
	{
		UE_LOG(LogTemp, Error, TEXT("UGEEC_Heal: Fail to find attribute %s or %s on target."), *PrimaryAttribute.AttributeName, *SecondaryAttribute.AttributeName);
		// todo: may need to remove gameplayEffect
		return;
	}

	if (HealLeft < 0)
	{
		HealLeft = HealAmount;
	}

	if (HealLeft == 0)
	{
		// todo: remove GE
		return;
	}

	float PrimaryAttributeValue = TargetASC->GetNumericAttribute(PrimaryAttribute);
	float MaxPrimaryAttributeValue = TargetASC->GetNumericAttribute(MaxPrimaryAttribute);
	float SecondaryAttributeValue = TargetASC->GetNumericAttribute(SecondaryAttribute);
	float MaxSecondaryAttributeValue = TargetASC->GetNumericAttribute(MaxSecondaryAttribute);
	if (PrimaryAttributeValue < MaxPrimaryAttributeValue)
	{
		float DeltaValue = FMath::Min(HealLeft, MaxPrimaryAttributeValue - PrimaryAttributeValue);
		HealLeft -= DeltaValue;
		TargetASC->SetNumericAttributeBase(PrimaryAttribute, PrimaryAttributeValue + DeltaValue);
	}
	else if (SecondaryAttributeValue < MaxSecondaryAttributeValue)
	{
		float DeltaValue = FMath::Min(HealLeft, 10);
		HealLeft -= DeltaValue;
		TargetASC->SetNumericAttributeBase(SecondaryAttribute, SecondaryAttributeValue + DeltaValue);
	}
	else
	{
		// todo: remove GE
	}

	if (HealLeft == 0)
	{
		// todo: remove GE
		return;
	}
}

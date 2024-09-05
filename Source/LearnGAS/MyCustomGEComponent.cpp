// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomGEComponent.h"

bool UMyCustomGEComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	UE_LOG(LogTemp, Display, TEXT("qtc UMyCustomGEComponent::CanGameplayEffectApply()"));
	return Super::CanGameplayEffectApply(ActiveGEContainer, GESpec);
}

bool UMyCustomGEComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	UE_LOG(LogTemp, Display, TEXT("qtc UMyCustomGEComponent::OnActiveGameplayEffectAdded()"));
	return Super::OnActiveGameplayEffectAdded(ActiveGEContainer, ActiveGE);
}

void UMyCustomGEComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	UE_LOG(LogTemp, Display, TEXT("qtc UMyCustomGEComponent::OnGameplayEffectExecuted()"));
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);
}

void UMyCustomGEComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	UE_LOG(LogTemp, Display, TEXT("qtc UMyCustomGEComponent::OnGameplayEffectApplied()"));
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
}

void UMyCustomGEComponent::OnGameplayEffectChanged() const
{
	UE_LOG(LogTemp, Display, TEXT("qtc UMyCustomGEComponent::OnGameplayEffectChanged()"));
	Super::OnGameplayEffectChanged();
}

EDataValidationResult UMyCustomGEComponent::IsDataValid(FDataValidationContext& Context) const
{
	UE_LOG(LogTemp, Display, TEXT("qtc UMyCustomGEComponent::IsDataValid()"));
	return Super::IsDataValid(Context);
}
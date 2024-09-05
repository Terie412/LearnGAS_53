// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAbilitySystemComponent.h"


// Sets default values for this component's properties
UMyAbilitySystemComponent::UMyAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UMyAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyAbilitySystemComponent::InhibitActiveGameplayEffect(FActiveGameplayEffectHandle ActiveGEHandle, bool bInhibit, bool bInvokeGameplayCueEvents)
{
	Super::InhibitActiveGameplayEffect(ActiveGEHandle, bInhibit, bInvokeGameplayCueEvents);

	
	OnActiveGameplayEffectInhibitionChanged.Broadcast(ActiveGEHandle, bInhibit);
	TObjectPtr<const UGameplayEffect> ActiveGE = GetActiveGameplayEffect(ActiveGEHandle)->Spec.Def;

	FActiveGameplayEffect* ActiveGE2 = const_cast<FActiveGameplayEffect*>(GetActiveGameplayEffect(ActiveGEHandle));
	
	// UE_LOG(LogTemp, Display, TEXT("qtc OnActiveGameplayEffectInhibitionChanged: %s, %s"), *ActiveGE->GetName(), bInhibit ? TEXT("true") : TEXT("false"));
}

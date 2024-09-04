// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

#include "MyAbilitySystemComponent.h"

AMyPlayerState::AMyPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	MyAttributeSet = CreateDefaultSubobject<UMyAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

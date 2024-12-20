// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWorldSubsystem.h"

FMyInputProcessor::FMyInputProcessor(class UMyWorldSubsystem* InMySubsystem)
{
	MySubsystem = InMySubsystem;
}

bool FMyInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if(MySubsystem)
	{
		return MySubsystem->HandleInput(InKeyEvent);
	}
	return false;
}

void FMyInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	
}

UMyWorldSubsystem::UMyWorldSubsystem()
{
	for (int i = 0; i < 100; i++)
	{
		UDebugObject* DebugObject = NewObject<UDebugObject>();
		DebugObject->FloatValue = i;
		DebugObjects.Add(DebugObject);
	}
}

void UMyWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// MyInputProcessor = MakeShareable(new FMyInputProcessor(this));
	// FSlateApplication::Get().RegisterInputPreProcessor(MyInputProcessor);
}

void UMyWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// if(MyInputProcessor)
	// {
	// 	FSlateApplication::Get().UnregisterInputPreProcessor(MyInputProcessor);
	// 	MyInputProcessor = nullptr;
	// }
}

bool UMyWorldSubsystem::HandleInput(const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogTemp, Display, TEXT("qtc GetKeyDown: %s"), *InKeyEvent.GetKey().ToString())
	return InKeyEvent.GetKey() == EKeys::K;
}

TArray<UDebugObject*> UMyWorldSubsystem::GetDebugObjects()
{
	TArray<UDebugObject*> OutDebugObjects;
	for (int i = 0; i < DebugCount; i++)
	{
		OutDebugObjects.Add(DebugObjects[i]);
	}
	return OutDebugObjects;

	// FSlateApplication::Get().RegisterInputPreProcessor()
}

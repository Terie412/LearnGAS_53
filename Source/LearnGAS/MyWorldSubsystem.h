// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"
#include "Subsystems/WorldSubsystem.h"
#include "MyWorldSubsystem.generated.h"

UCLASS(BlueprintType)
class UDebugObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FloatValue = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString StringValue = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FHitResult HitResult = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FHitResult> OutHitResults = {};
};

USTRUCT()
struct FHitResultView
{
	GENERATED_BODY()

	FHitResult CachedHitResult = {};

	UPROPERTY(EditDefaultsOnly)
	FVector ImpactPoint;

	FHitResultView()
	{
	}
};

class FMyInputProcessor: public IInputProcessor
{
public:
	FMyInputProcessor(class UMyWorldSubsystem* InMySubsystem);
	
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
private:
	class UMyWorldSubsystem* MySubsystem;
};

/**
 * 
 */
UCLASS(BlueprintType)
class LEARNGAS_API UMyWorldSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDebugObject> DebugObjectPtr = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<UDebugObject*> DebugObjects;

	UPROPERTY(BlueprintReadOnly)
	int DebugCount = 0;

	TSharedPtr<FMyInputProcessor> MyInputProcessor;

	UMyWorldSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;

	bool HandleInput(const FKeyEvent& InKeyEvent);

	UFUNCTION(BlueprintCallable)
	TArray<UDebugObject*> GetDebugObjects();
};



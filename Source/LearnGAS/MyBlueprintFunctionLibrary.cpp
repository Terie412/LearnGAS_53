// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#include "MyWidgetObject.h"

void UMyBlueprintFunctionLibrary::Test(UObject* WorldContextObject)
{
	UE_LOG(LogTemp, Display, TEXT("qtc GEngine=%p"), GEngine);
}

TArray<UObject*> UMyBlueprintFunctionLibrary::MakeObjectArray(int Size)
{
	TArray<UObject*> Result;
	for (int i = 0; i < Size; i++)
	{
		Result.Add(NewObject<UMyWidgetObject>());
	}

	return Result;
}

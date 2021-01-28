// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TowerBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API UTowerBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="String Formatting")
	static FString ZeroBuffer(float Number, int Numbers, int DecimalPlaces);

	UFUNCTION(BlueprintCallable, Category = "String Formatting")
		static FString FormatTimeAsMMSS(float Number, int DecimalPlaces);
	
};

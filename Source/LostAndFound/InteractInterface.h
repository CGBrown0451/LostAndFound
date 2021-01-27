// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

UCLASS(BlueprintType)
class LOSTANDFOUND_API UInteractInfo : public UObject
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadOnly)
	bool CanInteract = true;
	UPROPERTY(BlueprintReadOnly)
	bool HasTooltip;
	UPROPERTY(BlueprintReadOnly)
	FString Tooltip;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOSTANDFOUND_API IInteractInterface
{
	GENERATED_BODY()
public:
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    UInteractInfo* GetInteractInfo();
public:
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemLibrary.generated.h"

class UItem;
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItem> Class;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;
};

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API UItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Containers/Queue.h"
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

USTRUCT(BlueprintType)
struct FRandomPool
{
	GENERATED_BODY()
	TArray<int32> Pool;
};


USTRUCT(BlueprintType)
struct FCommissionData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Title;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> Requirements;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BonusTime;
};

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API UItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable)
	static bool PeekFirstValueInPool(const FRandomPool& Pool, int32& FirstValue);
	UFUNCTION(BlueprintCallable)
    static bool PopValueFromPool(UPARAM(ref) FRandomPool& Pool, int32& FirstValue);
	UFUNCTION(BlueprintCallable)
    static int32 PoolLength(const FRandomPool& Pool);
	UFUNCTION(BlueprintCallable)
    static bool QueueRangeOfValues(UPARAM(ref) FRandomPool& Pool, int32 Min, int32 Max);
};

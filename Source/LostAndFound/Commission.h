// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Commission.generated.h"

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API UCommission : public UObject
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> RequiredItems;

	float _TimeReward;
	float _TimeBegun;
	float _TimeLimit;
	bool _HasStartedTime;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBonusTimeReward();
	UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetTimeLimit();
	UFUNCTION(BlueprintCallable, BlueprintPure)
    bool GetRemainingTime(float& RemainingTime, bool& TimeRanOut);
	
	UFUNCTION(BlueprintCallable)
    float SetBonusTimeReward(float In);
	UFUNCTION(BlueprintCallable)
    float SetTimeLimit(float In);
	UFUNCTION(BlueprintCallable)
    bool StartTimingCommission();
};

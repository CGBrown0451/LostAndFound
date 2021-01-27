// Fill out your copyright notice in the Description page of Project Settings.


#include "Commission.h"

float UCommission::GetBonusTimeReward()
{
	return _TimeReward;
}

float UCommission::GetTimeLimit()
{
	return _TimeLimit;
}

bool UCommission::GetRemainingTime(float& RemainingTime, bool& TimeRanOut)
{
	RemainingTime = GetWorld()->GetTimeSeconds() - _TimeBegun;
	TimeRanOut = RemainingTime <= 0;
	return _HasStartedTime;
}

bool UCommission::InventoryHasAllItems(TMap<FName, int32>& OutsideInventory)
{
	for (auto& Item : RequiredItems)
	{
		auto Found = OutsideInventory.Find(Item.Key);
		if (Found)
		{
			if (Item.Value > *Found)
				return false;
		}
		else
			return false;
	}
	return true;
}

float UCommission::SetBonusTimeReward(float In)
{
	_TimeReward = In;
	return _TimeReward;
}

float UCommission::SetTimeLimit(float In)
{
	_TimeLimit = In;
	return _TimeLimit;
}

bool UCommission::StartTimingCommission()
{
	if (!_HasStartedTime)
	{
		_HasStartedTime = true;
		_TimeBegun = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}

void UCommission::RemoveRequiredItemsFromInventory(TMap<FName, int32>& OutsideInventory)
{
	for (auto& Item : RequiredItems)
	{
		auto Found = OutsideInventory.Find(Item.Key);
		if (Found)
		{
			if (Item.Value < *Found)
				OutsideInventory.Add(Item.Key, *Found - Item.Value);
			else 
				OutsideInventory.Remove(Item.Key);
		}
	}
}

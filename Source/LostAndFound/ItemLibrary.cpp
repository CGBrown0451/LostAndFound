// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLibrary.h"

bool UItemLibrary::PeekFirstValueInPool(const FRandomPool& Pool, int32& FirstValue)
{
	if (Pool.Pool.Num() > 0)
	{
		FirstValue = Pool.Pool[0];
		return true;
	}
	return false;
}

bool UItemLibrary::PopValueFromPool(UPARAM(ref) FRandomPool& Pool, int32& FirstValue)
{
	bool HasIndex = PeekFirstValueInPool(Pool, FirstValue);
	if (HasIndex)
		Pool.Pool.RemoveAt(0);
	return HasIndex;
}

int32 UItemLibrary::PoolLength(const FRandomPool& Pool)
{
	return Pool.Pool.Num();
}

bool UItemLibrary::QueueRangeOfValues(UPARAM(ref) FRandomPool& Pool, int32 Min, int32 Max)
{
	check(Min <= Max);
	TArray<int32> Arr;
	Arr.Init(0, Max - Min);
	for (int32 i = 0; i <= Max - Min; ++i)
	{
		Arr[i] = Min + i;
	}
	while (Arr.Num() > 0)
	{
		int32 Chosen = FMath::RandRange(0, Arr.Num());
		Pool.Pool.Add(Arr[Chosen]);
		Arr.RemoveAt(Chosen);
	}
	return true;
}

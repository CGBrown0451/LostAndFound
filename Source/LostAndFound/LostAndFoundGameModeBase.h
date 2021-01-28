// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LostAndFoundGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API ALostAndFoundGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float BeginningTime = 30.0f;
	
};

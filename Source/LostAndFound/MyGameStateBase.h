// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Commission.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

UENUM(BlueprintType)
enum EGameState
{
	GameState_MailRoom,
	GameState_Launching,
	GameState_InGame,
	GameState_PostGame
};
/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InGameTick(float DeltaSeconds);
	
public:
	UPROPERTY(BlueprintReadOnly)
	UCommission* CurrentCommission;
	UPROPERTY(BlueprintReadOnly)
	float TimeLeft;
	UPROPERTY(BlueprintReadOnly)
	float Score;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EGameState> GameState;

	//Adds Extra Time
	void AddExtraTime(float ExtraTime);

	//Sets Game State
	UFUNCTION(BlueprintCallable)
	void SetGameState(TEnumAsByte<EGameState> NewGameState){GameState = NewGameState;}

	//Gets a New Commission, returns whether the commission was added or not
	UFUNCTION(BlueprintCallable)
	bool GetNewCommission(UCommission* Commission);

	//Turns in the current commission, returns whether it can be completed or not
	bool TurnInCommission();
	
};

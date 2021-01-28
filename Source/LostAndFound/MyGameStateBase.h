// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Commission.h"
#include "MyPlayerController.h"
#include "ItemLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

class ASpawnLocation;
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
public:
	AMyGameStateBase();
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InGameTick(float DeltaSeconds);
	
	UFUNCTION()
	void OnStartLaunch();
	UFUNCTION()
	void OnLanded();

	AMyPlayerController* PlayerController;
	
public:
	UPROPERTY(BlueprintReadOnly)
	UCommission* CurrentCommission;
	UPROPERTY(BlueprintReadOnly)
	float TimeLeft;
	UPROPERTY(BlueprintReadOnly)
	float Score;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EGameState> GameState;

	UPROPERTY(BlueprintReadOnly)
	FCommissionData LastCommissionData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* CommissionsTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> CommissionRows;
	UPROPERTY()
	FRandomPool CommissionRandomPool;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ItemTable;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ASpawnLocation*> SpawnLocations;

	//Adds Extra Time
	void AddExtraTime(float ExtraTime);

	//Sets Game State
	UFUNCTION(BlueprintCallable)
	void SetGameState(TEnumAsByte<EGameState> NewGameState);

	//Gets a New Commission, returns whether the commission was added or not
	UFUNCTION(BlueprintCallable)
	bool GetNewCommission(UCommission* Commission);
	UFUNCTION(BlueprintCallable)
    void GenerateNewCommission();

	//Turns in the current commission, returns whether it can be completed or not
	bool TurnInCommission();
	
};
